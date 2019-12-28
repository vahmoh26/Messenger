#pragma once

#include <database/database.h>

#include "../item/item.h"

namespace server::core
{
	void login(const database::database& database, const item& request_item, item& response_item);
	void logout(const database::database& database, const item& request_item, item& response_item);
}