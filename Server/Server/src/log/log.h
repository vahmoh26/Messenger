#pragma once

#include <iostream>
#include <string>

namespace server::log
{
	using namespace std;

	void log(const string& file, const string& function, int line);
}