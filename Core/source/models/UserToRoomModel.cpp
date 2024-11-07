#include "UserToRoomModel.h"

namespace models
{
	UserToRoomModel::UserToRoomModel()
	{
		this->createTable
		(
			{
				{ "user_uuid", "TEXT NOT NULL" },
				{ "room_uuid", "TEXT NOT NULL" }
			}
		);
	}
}
