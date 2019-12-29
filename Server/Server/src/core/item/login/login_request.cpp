#include "login_request.h"

#include <sstream>

#include <boost/iostreams/stream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace server::core
{
	using namespace boost;

	login_request::login_request()
	{
		_type = type::login;
	}

	login_request::login_request(const string& ip, const string& username, const string& password) : login_request()
	{
		set_ip(ip);
		set_username(username);
		set_password(password);
	}

	login_request::login_request(const protocol::package& package) : login_request()
	{
		iostreams::array_source array_source(package.get_buffer().data(), package.get_buffer().size());
		iostreams::stream<boost::iostreams::array_source> stream(array_source);
		archive::text_iarchive text_iarchive(stream);

		text_iarchive >> _type;
		text_iarchive >> _username;
		text_iarchive >> _password;
	}

	login_request::~login_request()
	{
	}

	void login_request::set_username(const string& username)
	{
		_username = username;
	}

	void login_request::set_password(const string& password)
	{
		_password = password;
	}

	const string& login_request::get_username() const
	{
		return _username;
	}

	const string& login_request::get_password() const
	{
		return _password;
	}

	protocol::package login_request::to_package()
	{
		stringstream string_stream;
		archive::text_oarchive text_oarchive(string_stream);
		text_oarchive << _type;
		text_oarchive << _username;
		text_oarchive << _password;

		auto string = string_stream.str();

		return { _ip, vector<char>(string.c_str(), string.c_str() + string.size()) };
	}
}