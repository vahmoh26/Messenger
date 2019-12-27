#include "response.h"

namespace server::core
{
	response::response()
	{
		_type = type::unknown;
	}

	response::response(type type) : response()
	{
		set_type(type);
	}

	response::~response()
	{
	}

	void response::set_type(type type)
	{
		_type = type;
	}

	response::type response::get_type()
	{
		return _type;
	}

	bool response::valid()
	{
		return _type > type::unknown && _type <= type::logout;
	}

	protocol::package response::to_package(const response& response)
	{
		return {};
	}
}