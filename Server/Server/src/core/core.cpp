#include "core.h"

#include <mutex>
#include <optional>

#include <log/log.h>

namespace server::core
{
	mutex requests_mutex;
	mutex responses_mutex;

	core::core()
	{
		initialized = false;
		started = false;
		service_threads_stop = false;
		request_thread_stop = false;
		response_thread_stop = false;
	}

	core::~core()
	{
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
	}

	bool core::initialize()
	{
		if (initialized)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		if (!protocol.initialize())
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		initialized = true;

		return true;
	}

	bool core::start()
	{
		if (!initialized || started)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		if (!protocol.start())
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		for (auto& service_thread : service_threads)
		{
			service_thread = thread(&core::service_thread_func, this);
		}

		request_thread = thread(&core::request_thread_func, this);
		response_thread = thread(&core::response_thread_func, this);

		started = true;

		return true;
	}

	void core::service_thread_func()
	{
		optional<request> request;
		response response;

		while (!service_threads_stop)
		{
			requests_mutex.lock();

			if (!requests.empty())
			{
				request = requests.front();
				requests.pop();
			}

			requests_mutex.unlock();

			if (request.has_value() && request.value().valid())
			{
				switch (request.value().get_type())
				{
				case request::type::login:
					login(request.value(), response);
					break;
				case request::type::logout:
					logout(request.value(), response);
					break;
				default:
					break;
				}

				responses_mutex.lock();
				responses.push(response);
				responses_mutex.unlock();

				request.reset();
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
					requests_mutex.lock();
					requests.push(request::from_package(packages.front()));
					requests_mutex.unlock();

					packages.pop();
				}
			}
		}
	}

	void core::response_thread_func()
	{
		optional<response> response;

		while (!response_thread_stop)
		{
			responses_mutex.lock();

			if (!responses.empty())
			{
				response = responses.front();
				responses.pop();
			}

			responses_mutex.unlock();

			if (response.has_value())
			{
				protocol.send(response::to_package(response.value()));

				response.reset();
			}
		}
	}
}