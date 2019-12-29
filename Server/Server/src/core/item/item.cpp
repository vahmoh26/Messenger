#include "item.h"

namespace server::core
{
	item::item()
	{
		_type = type::unknown;
	}

	item::item(type type) : item()
	{
		set_type(type);
	}

	item::item(const protocol::package& package)
	{
	}

	item::~item()
	{
	}

	void item::set_type(type type)
	{
		_type = type;
	}

	void item::set_ip(const string& ip)
	{
		_ip = ip;
	}

	item::type item::get_type()
	{
		return _type;
	}

	const string& item::get_ip() const
	{
		return _ip;
	}

	bool item::valid()
	{
		return _type > type::unknown && _type <= type::logout;
	}

	protocol::package item::to_package()
	{
		return {};
	}
}