#pragma once

#include "request/request.h"
#include "response/response.h"

namespace server::core
{
	void login(const request& request, response& response);
	void logout(const request& request, response& response);
}