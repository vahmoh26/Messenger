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
			LOG();

			return false;
		}

		opened = true;

		return true;
	}

	bool database::close()
	{
		if (!opened)
		{
			LOG();

			return false;
		}

		return true;
	}
}