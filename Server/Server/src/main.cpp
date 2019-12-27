#include <core/core.h>

int main(int argc, char* argv[])
{
	server::core::core core;

	core.initialize();
	core.start();

	while (true);

	return 0;
}