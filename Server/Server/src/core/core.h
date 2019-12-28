#pragma once

#include <array>

#include <config/config.h>
#include <database/database.h>
#include <protocol/protocol.h>

#include "item/item.h"

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

		bool terminate();

	private:
		void service_thread_func(uint16_t index);
		void request_thread_func();
		void response_thread_func();

		bool initialized;
		bool started;
		bool terminated;
		array<thread, config::core::service_threads_number> service_threads;
		thread request_thread;
		thread response_thread;
		atomic_bool service_threads_stop;
		atomic_bool request_thread_stop;
		atomic_bool response_thread_stop;
		array<database::database, config::core::service_threads_number> databases;
		protocol::protocol protocol;
		queue<item> request_items;
		queue<item> response_items;
	};
}