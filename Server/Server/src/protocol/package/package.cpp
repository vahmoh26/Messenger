#include "package.h"

namespace server::protocol
{
	package::package()
	{
	}

	package::package(const string& ip, const vector<char>& buffer) : package()
	{
		set_ip(ip);
		set_buffer(buffer);
	}

	package::~package()
	{
	}

	void package::set_ip(const string& ip)
	{
		_ip = ip;
	}

	void package::set_buffer(const vector<char>& buffer)
	{
		_buffer = buffer;
	}

	const string& package::get_ip()
	{
		return _ip;
	}

	const vector<char>& package::get_buffer()
	{
		return _buffer;
	}
}