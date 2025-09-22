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
		return "CREATE TABLE IF NOT EXISTS rooms(id INTEGER PRIMARY KEY, invite_link TEXT NOT NULL, name TEXT NOT NULL, uuid VARCHAR(36) NOT NULL UNIQUE CHECK (length(uuid) <= 36))";
	}

	inline constexpr std::string_view createUsersQuery()
	{
		return "CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY, room_id INTEGER, name TEXT NOT NULL, role TEXT NOT NULL, uuid VARCHAR(36) NOT NULL UNIQUE CHECK (length(uuid) <= 36), FOREIGN KEY (room_id) REFERENCES rooms(id))";
	}
}
