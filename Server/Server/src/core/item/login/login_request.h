#pragma once

#include "../item.h"

namespace server::core
{
	using namespace std;

	class login_request : public item
	{
	public:
		login_request();
		login_request(const string& ip, const string& username, const string& password);
		login_request(const protocol::package& package);
		~login_request();

		void set_username(const string& username);
		void set_password(const string& password);

		const string& get_username() const;
		const string& get_password() const;

		protocol::package to_package();

	private:
		string _username;
		string _password;
	};
}