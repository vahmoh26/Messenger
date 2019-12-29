#include "item.h"

#include <sstream>

#include <boost/iostreams/stream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace server::core
{
	using namespace boost;

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
		iostreams::array_source array_source(package.get_buffer().data(), package.get_buffer().size());
		iostreams::stream<boost::iostreams::array_source> stream(array_source);
		archive::text_iarchive text_iarchive(stream);

		text_iarchive >> _type;
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