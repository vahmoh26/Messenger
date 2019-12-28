#pragma once

namespace server::database
{
	class database
	{
	public:
		database();
		~database();

		bool open();
		bool close();

	private:
		bool opened;
	};
}