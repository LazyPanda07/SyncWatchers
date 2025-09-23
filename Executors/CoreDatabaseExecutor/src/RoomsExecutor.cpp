#include "RoomsExecutor.h"

#include <filesystem>

#include <Utility/WebFrameworkUtility.hpp>

#include "CreateTableQueries.h"

namespace executors
{
	std::string RoomsExecutor::inviteLink = "";

	std::string_view RoomsExecutor::getBaseInviteLink()
	{
		return RoomsExecutor::inviteLink;
	}

	void RoomsExecutor::init(const framework::utility::ExecutorSettings& settings)
	{
		RoomsExecutor::inviteLink = settings.getInitParameters().get<std::string>("inviteLink");
	}

	void RoomsExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table users = request.getTable(":memory:", "users");
		framework::SQLResult sqlResult = users.execute
		(
			"SELECT name FROM users WHERE room_id = (SELECT id FROM rooms WHERE uuid = ?)",
			{ framework::SQLValue(request.getQueryParameters().at("room_uuid")) }
		);
		std::vector<framework::JSONObject> result;

		for (const auto& row : sqlResult)
		{
			for (const auto& [_, value] : row)
			{
				framework::utility::appendArray(result, value.get<std::string>());
			}
		}

		response.setBody(framework::JSONBuilder().append("users", result));
	}

	void RoomsExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Database database = request.getOrCreateDatabase(":memory:");
		framework::Table rooms = database.getOrCreateTable("rooms", database::createRoomsQuery());

		std::string roomName = framework::JSONParser(request.getBody()).get<std::string>("name");
		std::string roomInviteLink = std::format("{}/{}", inviteLink, framework::utility::uuid::generateUUID());

		framework::SQLResult test = rooms.execute
		(
			"INSERT INTO rooms (invite_link, name, uuid) VALUES (?, ?, ?)",
			{ framework::SQLValue(roomInviteLink), framework::SQLValue(roomName), framework::SQLValue(framework::utility::uuid::generateUUID()) }
		);

		response.setBody(framework::JSONBuilder().append("inviteLink", roomInviteLink));
	}

	void RoomsExecutor::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table rooms = request.getTable(":memory:", "rooms");
		framework::Table users = request.getTable(":memory:", "users");
		framework::JSONParser parser(request.getBody());
		std::string uuid = parser.get<std::string>("room_uuid");

		users.execute
		(
			"DELETE from users WHERE room_id = (SELECT id FROM rooms WHERE uuid = ?)",
			{ framework::SQLValue(uuid) }
		);

		rooms.execute
		(
			"DELETE FROM rooms WHERE id = (SELECT id FROM rooms WHERE uuid = ?)",
			{ framework::SQLValue(uuid) }
		);

		std::filesystem::remove_all(std::filesystem::current_path() / "assets" / uuid);

		response.setBody("Room deleted");
	}

	DEFINE_EXECUTOR(RoomsExecutor);
}
