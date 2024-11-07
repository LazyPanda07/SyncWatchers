#pragma once

#include <SQLite3/SQLiteDatabaseModel.h>

namespace models
{
	class RoomModel : public framework::sqlite::SQLiteDatabaseModel
	{
		DECLARE_DATABASE_AND_TABLE_NAME("core", "room");

	public:
		RoomModel();

		~RoomModel() = default;
	};
}
