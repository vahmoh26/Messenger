#pragma once

#include <protocol/protocol.h>

namespace server::core
{
	class response
	{
	public:
		enum class type
		{
			unknown,
			login,
			logout
		};

		response();
		response(type type);
		~response();

		void set_type(type type);

		type get_type();

		bool valid();

		static protocol::package to_package(const response& response);

	private:
		type _type;
	};
}