#pragma once

#include <string_view>

namespace database
{
	constexpr std::string_view createRoomsQuery();

	constexpr std::string_view createUsersQuery();
}

namespace database
{
	inline constexpr std::string_view createRoomsQuery()
	{
		return "CREATE TABLE rooms(id INTEGER PRIMARY KEY, invite_link TEXT NOT NULL, name TEXT NOT NULL)";
	}

	inline constexpr std::string_view createUsersQuery()
	{
		return "CREATE TABLE users(id INTEGER PRIMARY KEY, room_id INTEGER, name TEXT NOT NULL, role TEXT NOT NULL, FOREIGN KEY (room_id) REFERENCES rooms(id))";
	}
}
