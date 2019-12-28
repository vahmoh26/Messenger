#pragma once

#include <protocol/protocol.h>

namespace server::core
{
	class item
	{
	public:
		enum class type
		{
			unknown,
			login,
			logout
		};

		item();
		item(type type);
		item(const protocol::package& package);
		~item();

		void set_type(type type);

		type get_type();

		bool valid();

		virtual protocol::package to_package();

	private:
		type _type;
	};
}