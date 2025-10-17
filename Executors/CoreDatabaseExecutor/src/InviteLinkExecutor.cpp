#include "InviteLinkExecutor.h"

#include <random>
#include <fstream>

#include <Utility/WebFrameworkUtility.hpp>

#include "RoomsExecutor.h"
#include "CreateTableQueries.h"
#include "Utils.h"
#include "Events/OnInviteEvent.h"

namespace executors
{
	InviteLinkExecutor::InviteLinkExecutor() :
		useHTTPS(false)
	{

	}

	void InviteLinkExecutor::init(const framework::utility::ExecutorSettings& settings)
	{
		std::string data = (std::ostringstream() << std::ifstream("config.json").rdbuf()).str();
		framework::JSONParser parser(data);

		useHTTPS = parser.get<bool>("useHTTPS", true);
	}

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

	void InviteLinkExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table rooms = request.getTable(":memory:", "rooms");
		const framework::HTTPRequest::HeadersMap& headers = request.getHeaders();
		std::string host;

		if (auto it = headers.find("Host"); it == headers.end())
		{
			response.setBody("Can't find Host header");

			response.setResponseCode(framework::ResponseCodes::badRequest);

			return;
		}
		else
		{
			host = it->second;
		}

		std::string link = std::format("{}://{}/invite_link/{}", (useHTTPS ? "https" : "http"), host, request.getRouteParameter<std::string>("link"));

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
			std::string role = "default";
			std::string userUUID = framework::utility::uuid::generateUUID();
			std::string roomUUID = row.at("uuid").get<std::string>();

			request.getJSON().tryGet<std::string>("role", role);

			users.execute
			(
				"INSERT INTO users (room_id, name, role, uuid) VALUES (?, ?, ?, ?)",
				{ framework::SQLValue(id), framework::SQLValue(userName), framework::SQLValue(role), framework::SQLValue(userUUID) }
			);

			result["roomUUID"] = roomUUID;
			result["userName"] = userName;
			result["userUUID"] = userUUID;
			result["role"] = role;

			utils::notify<events::OnInviteEvent>(roomUUID, userName);

			response.setResponseCode(framework::ResponseCodes::created);
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
