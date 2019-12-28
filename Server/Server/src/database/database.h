#pragma once

namespace server::database
{
	class database
	{
	public:
		database();
		~database();

		bool open();

	private:
		bool opened;
	};
}