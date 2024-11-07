#pragma once

#include <SQLite3/SQLiteDatabaseModel.h>

namespace models
{
	class UserModel : public framework::sqlite::SQLiteDatabaseModel
	{
		DECLARE_DATABASE_AND_TABLE_NAME("core", "user");

	public:
		UserModel();

		~UserModel() = default;
	};
}
