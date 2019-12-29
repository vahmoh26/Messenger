#include "core.h"

#include <mutex>
#include <optional>

#include <log/log.h>

#include "service/service.h"

namespace server::core
{
	mutex request_items_mutex;
	mutex response_items_mutex;

	core::core()
	{
		initialized = false;
		started = false;
		terminated = false;
		service_threads_stop = false;
		request_thread_stop = false;
		response_thread_stop = false;
	}

	core::~core()
	{
	}

	bool core::initialize()
	{
		if (initialized || terminated)
		{
			LOG();

			return false;
		}

		for (auto& database : databases)
		{
			if (!database.open())
			{
				LOG();

				return false;
			}
		}

		if (!protocol.initialize())
		{
			LOG();

			return false;
		}

		initialized = true;

		return true;
	}

	bool core::start()
	{
		if (!initialized || started)
		{
			LOG();

			return false;
		}

		if (!protocol.start())
		{
			LOG();

			return false;
		}

		for (auto& service_thread : service_threads)
		{
			service_thread = thread(&core::service_thread_func, this, static_cast<uint16_t>(&service_thread - &service_threads[0]));
		}

		request_thread = thread(&core::request_thread_func, this);
		response_thread = thread(&core::response_thread_func, this);

		started = true;

		return true;
	}

	bool core::terminate()
	{
		auto result = true;

		if (started)
		{
			service_threads_stop = true;
			request_thread_stop = true;
			response_thread_stop = true;

			for (auto& service_thread : service_threads)
			{
				if (service_thread.joinable())
				{
					service_thread.join();
				}
			}

			if (request_thread.joinable())
			{
				request_thread.join();
			}

			if (response_thread.joinable())
			{
				response_thread.join();
			}
		}

		if (initialized)
		{
			if (!protocol.terminate())
			{
				result = false;

				LOG();
			}

			for (auto& database : databases)
			{
				if (!database.close())
				{
					LOG();

					result = false;
				}
			}
		}

		terminated = true;

		return result;
	}

	void core::service_thread_func(uint16_t index)
	{
		auto& database = databases[index];

		optional<item> request_item;
		item response_item;

		while (!service_threads_stop)
		{
			request_items_mutex.lock();

			if (!request_items.empty())
			{
				request_item = request_items.front();
				request_items.pop();
			}

			request_items_mutex.unlock();

			if (request_item.has_value() && request_item.value().valid())
			{
				switch (request_item.value().get_type())
				{
				case item::type::login:
					login(database, request_item.value(), response_item);
					break;
				case item::type::logout:
					logout(database, request_item.value(), response_item);
					break;
				default:
					break;
				}

				response_items_mutex.lock();
				response_items.push(response_item);
				response_items_mutex.unlock();

				request_item.reset();
			}
		}
	}

	void core::request_thread_func()
	{
		queue<protocol::package> packages;

		while (!request_thread_stop)
		{
			if (protocol.receive(packages))
			{
				while (!packages.empty())
				{
					request_items_mutex.lock();

					switch (item(packages.front()).get_type())
					{
					case item::type::login:
						request_items.push(*dynamic_cast<item*>(&login_request(packages.front())));
						break;
					case item::type::logout:
						break;
					default:
						break;
					}

					request_items_mutex.unlock();

					packages.pop();
				}
			}
		}
	}

	void core::response_thread_func()
	{
		optional<item> response_item;

		while (!response_thread_stop)
		{
			response_items_mutex.lock();

			if (!response_items.empty())
			{
				response_item = response_items.front();
				response_items.pop();
			}

			response_items_mutex.unlock();

			if (response_item.has_value())
			{
				protocol.send(response_item.value().to_package());

				response_item.reset();
			}
		}
	}
}