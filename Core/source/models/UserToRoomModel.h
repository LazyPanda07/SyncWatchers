#pragma once

#include <SQLite3/SQLiteDatabaseModel.h>

namespace models
{
	class UserToRoomModel : public framework::sqlite::SQLiteDatabaseModel
	{
		DECLARE_DATABASE_AND_TABLE_NAME("core", "user_to_room");

	public:
		UserToRoomModel();

		~UserToRoomModel() = default;
	};
}
