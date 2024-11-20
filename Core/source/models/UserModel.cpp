#include "UserModel.h"

namespace models
{
	UserModel::UserModel()
	{
		this->createTable
		(
			{
				{ "uuid", "TEXT UNIQUE NOT NULL" },
				{ "name", "TEXT NOT NULL" },
				{ "ip", "TEXT NOT NULL" },
				{ "room_uuid", "TEXT NOT NULL" },
				{ "FOREIGN KEY(room_uuid)", "REFERENCES room(uuid)" }
			}
		);
	}
}
