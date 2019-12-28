#include "init.h"

#include <log/log.h>

namespace server::init
{
	bool stop;
	server::core::core core;

	BOOL WINAPI console_ctrl_handler(DWORD signal)
	{
		if (signal == CTRL_C_EVENT)
		{
			stop = true;
		}

		return TRUE;
	}

	int execute()
	{
		stop = false;

		if (!SetConsoleCtrlHandler(console_ctrl_handler, TRUE))
		{
			LOG();

			return EXIT_FAILURE;
		}

		if (!core.initialize())
		{
			LOG();

			return EXIT_FAILURE;
		}

		if (!core.start())
		{
			LOG();

			return EXIT_FAILURE;
		}

		while (!stop);

		if (!core.terminate())
		{
			LOG();

			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}
}