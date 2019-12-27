#include "protocol.h"

#include <mutex>
#include <optional>

#include <ws2tcpip.h>

#include <config/config.h>
#include <log/log.h>

namespace server::protocol
{
	mutex clients_mutex;
	mutex receive_packages_mutex;
	mutex send_packages_mutex;

	protocol::protocol()
	{
		initialized = false;
		started = false;
		receive_thread_stop = false;
		send_thread_stop = false;
		server = INVALID_SOCKET;
	}

	protocol::~protocol()
	{
		if (started)
		{
			receive_thread_stop = true;
			send_thread_stop = true;

			if (receive_thread.joinable())
			{
				receive_thread.join();
			}

			if (send_thread.joinable())
			{
				send_thread.join();
			}

			for (auto const& [ip, socket] : clients)
			{
				if (::closesocket(socket) == SOCKET_ERROR)
				{
					log::log(__FILE__, __FUNCTION__, __LINE__);
				}
			}
		}

		if (initialized)
		{
			if (::closesocket(server) == SOCKET_ERROR)
			{
				log::log(__FILE__, __FUNCTION__, __LINE__);
			}

			if (::WSACleanup() != NO_ERROR)
			{
				log::log(__FILE__, __FUNCTION__, __LINE__);
			}
		}
	}

	bool protocol::initialize()
	{
		if (initialized)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		WSADATA wsadata;
		if (::WSAStartup(MAKEWORD(2, 2), &wsadata) != NO_ERROR)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		server = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (server == INVALID_SOCKET)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		u_long mode = 1;
		if (::ioctlsocket(server, FIONBIO, &mode) == SOCKET_ERROR)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		initialized = true;

		return true;
	}

	bool protocol::start()
	{
		if (!initialized || started)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(config::protocol::port);

		if (::bind(server, (SOCKADDR*)& addr, sizeof(addr)) == SOCKET_ERROR)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		if (::listen(server, SOMAXCONN) == SOCKET_ERROR)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		receive_thread = thread(&protocol::receive_thread_func, this);
		send_thread = thread(&protocol::send_thread_func, this);

		started = true;

		return true;
	}

	bool protocol::receive(queue<package>& packages)
	{
		if (!started)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		receive_packages_mutex.lock();
		packages = move(receive_packages);
		receive_packages_mutex.unlock();

		return true;
	}

	bool protocol::send(const package& package)
	{
		if (!started)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		send_packages_mutex.lock();
		send_packages.push(package);
		send_packages_mutex.unlock();

		return true;
	}

	void protocol::receive_thread_func()
	{
		SOCKET client;
		SOCKADDR_IN addr;
		int addr_size = sizeof(addr);

		int recv;
		char buffer[1024];

		while (!receive_thread_stop)
		{
			client = ::accept(server, (SOCKADDR*)& addr, &addr_size);
			if (client != INVALID_SOCKET)
			{
				clients_mutex.lock();
				clients.insert_or_assign(addr_to_ip(addr), client);
				clients_mutex.unlock();
			}

			clients_mutex.lock();

			for (auto const& [ip, socket] : clients)
			{
				recv = ::recv(socket, buffer, 1024, 0);
				if (recv > 0)
				{
					receive_packages_mutex.lock();
					receive_packages.push({ ip, vector<char>(buffer, buffer + recv) });
					receive_packages_mutex.unlock();
				}
			}

			clients_mutex.unlock();
		}
	}

	void protocol::send_thread_func()
	{
		optional<package> package;
		SOCKET client;

		while (!send_thread_stop)
		{
			send_packages_mutex.lock();

			if (!send_packages.empty())
			{
				package = send_packages.front();
				send_packages.pop();
			}

			send_packages_mutex.unlock();

			if (package.has_value())
			{
				client = ip_to_socket(package.value().get_ip());
				if (client != INVALID_SOCKET)
				{
					if (::send(client, package.value().get_buffer().data(), static_cast<int>(package.value().get_buffer().size()), 0) == SOCKET_ERROR)
					{
						log::log(__FILE__, __FUNCTION__, __LINE__);
					}
				}

				package.reset();
			}
		}
	}

	SOCKET protocol::ip_to_socket(const string & ip)
	{
		lock_guard<mutex> lock(clients_mutex);

		auto client = clients.find(ip);
		if (client == clients.end())
		{
			return INVALID_SOCKET;
		}

		return client->second;
	}

	string protocol::addr_to_ip(const SOCKADDR_IN & addr)
	{
		char ip[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);

		return ip;
	}
}