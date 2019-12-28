#include "log.h"

namespace server::log
{
	void log(const string& file, const string& function, int line)
	{
		cout << "file: " << file << endl;
		cout << "function: " << function << endl;
		cout << "line: " << line << endl;
	}
}