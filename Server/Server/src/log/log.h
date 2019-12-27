#pragma once

#include <iostream>
#include <string>

namespace server::log
{
	using namespace std;

	static void log(const string& file, const string& function, int line)
	{
		cout << "file: " << file << endl;
		cout << "function: " << function << endl;
		cout << "line: " << line << endl;
	}
}