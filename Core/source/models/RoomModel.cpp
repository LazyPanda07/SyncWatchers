#include "RoomModel.h"

RoomModel::RoomModel()
{
	this->createTable
	(
		{
			{ "uuid", "TEXT UNIQUE NOT NULL" },
			{ "name", "TEXT NOT NULL" }
		}
	);
}
