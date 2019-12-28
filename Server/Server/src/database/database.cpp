#include "database.h"

#include <log/log.h>

namespace server::database
{
	database::database()
	{
		opened = false;
	}

	database::~database()
	{
	}

	bool database::open()
	{
		if (opened)
		{
			log::log(__FILE__, __FUNCTION__, __LINE__);

			return false;
		}

		return true;
	}
}