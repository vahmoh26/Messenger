#pragma once

#include <cstdint>

namespace server::config
{
	namespace core
	{
		constexpr uint16_t service_threads_number = 3;
	}

	namespace protocol
	{
		constexpr uint16_t port = 6533;
	}
}