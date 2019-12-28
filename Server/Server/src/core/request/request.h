#pragma once

#include <protocol/protocol.h>

namespace server::core
{
	class request
	{
	public:
		enum class type
		{
			unknown,
			login,
			logout
		};

		request();
		request(type type);
		~request();

		void set_type(type type);

		type get_type();

		bool valid();

		static request from_package(const protocol::package& package);

	private:
		type _type;
	};
}