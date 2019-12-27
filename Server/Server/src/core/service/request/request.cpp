#include "request.h"

namespace server::core
{
	request::request()
	{
		_type = type::unknown;
	}

	request::request(type type) : request()
	{
		set_type(type);
	}

	request::~request()
	{
	}

	void request::set_type(type type)
	{
		_type = type;
	}

	request::type request::get_type()
	{
		return _type;
	}

	bool request::valid()
	{
		return _type > type::unknown && _type <= type::logout;
	}

	request request::from_package(const protocol::package& package)
	{
		return {};
	}
}