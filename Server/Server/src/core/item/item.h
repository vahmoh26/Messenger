#pragma once

#include <protocol/protocol.h>

namespace server::core
{
	using namespace std;

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
		void set_ip(const string& ip);

		type get_type();
		const string& get_ip() const;

		bool valid();

		virtual protocol::package to_package();

	protected:
		type _type;
		string _ip;
	};
}