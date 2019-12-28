#pragma once

#include <thread>
#include <atomic>
#include <unordered_map>
#include <queue>

#include <WinSock2.h>

#include "package/package.h"

namespace server::protocol
{
	using namespace std;

	class protocol
	{
	public:
		protocol();
		~protocol();

		bool initialize();
		bool start();

		bool terminate();

		bool receive(queue<package>& packages);
		bool send(const package& package);

	private:
		void receive_thread_func();
		void send_thread_func();

		SOCKET ip_to_socket(const string& ip);

		static string addr_to_ip(const SOCKADDR_IN& addr);

		bool initialized;
		bool started;
		bool terminated;
		thread receive_thread;
		thread send_thread;
		atomic_bool receive_thread_stop;
		atomic_bool send_thread_stop;
		SOCKET server;
		unordered_map<string, SOCKET> clients;
		queue<package> receive_packages;
		queue<package> send_packages;
	};
}