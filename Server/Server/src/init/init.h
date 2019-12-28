#pragma once

#include <core/core.h>

namespace server::init
{
	extern bool stop;
	extern server::core::core core;

	int execute();
}