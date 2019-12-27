#pragma once

#include <array>

#include <config/config.h>
#include <protocol/protocol.h>

#include "service/service.h"

namespace server::core
{
	using namespace std;

	class core
	{
	public:
		core();
		~core();

		bool initialize();
		bool start();

	private:
		void service_thread_func();
		void request_thread_func();
		void response_thread_func();

		bool initialized;
		bool started;
		array<thread, config::core::service_threads_number> service_threads;
		thread request_thread;
		thread response_thread;
		atomic_bool service_threads_stop;
		atomic_bool request_thread_stop;
		atomic_bool response_thread_stop;
		protocol::protocol protocol;
		queue<request> requests;
		queue<response> responses;
	};
}