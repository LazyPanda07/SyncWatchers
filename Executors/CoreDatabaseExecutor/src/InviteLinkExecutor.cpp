#include "InviteLinkExecutor.h"

#include <random>

#include <Utility/WebFrameworkUtility.hpp>

#include "RoomsExecutor.h"
#include "CreateTableQueries.h"

namespace executors
{
	std::string InviteLinkExecutor::generateDefaultName()
	{
		std::mt19937 random(static_cast<uint32_t>(time(nullptr)));
		std::string result(16, '\0');

		for (size_t i = 0; i < 16; i++)
		{
			result[i] = random() % 26 + 65;
		}

		return result;
	}

	void InviteLinkExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table rooms = request.getTable(":memory:", "rooms");
		std::string link = std::format("{}/{}", RoomsExecutor::getBaseInviteLink(), request.getRouteParameter<std::string>("link"));
		framework::SQLResult sqlResult = rooms.execute
		(
			"SELECT id, uuid FROM rooms WHERE invite_link = ?",
			{ framework::SQLValue(link) }
		);

		if (sqlResult.size())
		{
			framework::Table users = request.getOrCreateTable(":memory:", "users", database::createUsersQuery());
			const framework::SQLResult::Row& row = *sqlResult.begin();
			int64_t id = row.at("id").get<int64_t>();
			std::string userName = InviteLinkExecutor::generateDefaultName();
			framework::JSONBuilder result;
			framework::JSONParser parser(request.getBody());
			std::string role = "default";
			std::string userUUID = framework::utility::uuid::generateUUID();

			parser.tryGet<std::string>("role", role);

			users.execute
			(
				"INSERT INTO users (room_id, name, role, uuid) VALUES (?, ?, ?, ?)",
				{ framework::SQLValue(id), framework::SQLValue(userName), framework::SQLValue(role), framework::SQLValue(userUUID) }
			);

			result["roomUUID"] = row.at("uuid").get<std::string>();
			result["userName"] = userName;
			result["userUUID"] = userUUID;
			result["role"] = role;

			response.setBody(result);
		}
		else
		{
			response.setResponseCode(framework::ResponseCodes::notFound);

			response.setBody("Can't join to room");
		}
	}

	DEFINE_EXECUTOR(InviteLinkExecutor);
}
