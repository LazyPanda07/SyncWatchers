#pragma once

#include <string_view>

namespace database
{
	constexpr std::string_view createRoomsQuery();

	constexpr std::string_view createUsersQuery();

	constexpr std::string_view createContentQuery();
}

namespace database
{
	inline constexpr std::string_view createRoomsQuery()
	{
		return "CREATE TABLE IF NOT EXISTS rooms(id INTEGER PRIMARY KEY, invite_link TEXT NOT NULL, name TEXT NOT NULL, uuid VARCHAR(36) NOT NULL UNIQUE CHECK (length(uuid) <= 36))";
	}

	inline constexpr std::string_view createUsersQuery()
	{
		return "CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY, room_id INTEGER, name TEXT NOT NULL, role TEXT NOT NULL CHECK (role IN ('owner', 'link_poster', 'default')), uuid VARCHAR(36) NOT NULL UNIQUE CHECK (length(uuid) <= 36), FOREIGN KEY (room_id) REFERENCES rooms(id))";
	}

	inline constexpr std::string_view createContentQuery()
	{
		return "CREATE TABLE IF NOT EXISTS content(id INTEGER PRIMARY KEY, name TEXT NOT NULL, room_id INTEGER, upload_user_id INTEGER, FOREIGN KEY (room_id) REFERENCES rooms(id), FOREIGN KEY (upload_user_id) REFERENCES users(id))";
	}
}
