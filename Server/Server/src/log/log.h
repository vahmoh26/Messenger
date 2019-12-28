#pragma once

#include <iostream>
#include <string>

#define LOG() server::log::log(__FILE__, __FUNCTION__, __LINE__)

namespace server::log
{
	using namespace std;

	void log(const string& file, const string& function, int line);
}