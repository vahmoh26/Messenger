#include <protocol/protocol.h>

int main(int argc, char* argv[])
{
	server::protocol::protocol protocol;

	protocol.initialize();
	protocol.start();

	while (true);

	return 0;
}