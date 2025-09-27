#include "UsersExecutor.h"

#include "CreateTableQueries.h"
#include "Utils.h"
#include "Events/OnUpdateRoleEvent.h"
#include "Events/OnUserNameUpdateEvent.h"

namespace executors
{
	void UsersExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table users = request.getTable(":memory:", "users");
		std::string uuid = request.getQueryParameters().at("user_uuid");
		framework::JSONBuilder result;

		framework::SQLResult sqlResult = users.execute
		(
			"SELECT id, name, role FROM users WHERE uuid = ?",
			{ framework::SQLValue(uuid) }
		);

		if (sqlResult.size())
		{
			try
			{
				request.getOrCreateTable(":memory:", "content", database::createContentQuery());

				const framework::SQLResult::Row& user = *sqlResult.begin();
				framework::Table content = request.getTable(":memory:", "content");
				std::vector<framework::JSONObject> uploadedContent;

				framework::SQLResult sqlContent = content.execute
				(
					"SELECT name FROM content WHERE upload_user_id = ?",
					{ framework::SQLValue(user.at("id").get<int64_t>()) }
				);

				for (const auto& row : sqlContent)
				{
					for (const auto& [_, contentName] : row)
					{
						framework::utility::appendArray(uploadedContent, contentName.get<std::string>());
					}
				}

				result["name"] = user.at("name").get<std::string>();
				result["role"] = user.at("role").get<std::string>();
				result["uploadedContent"] = uploadedContent;

				response.setBody(result);
			}
			catch (const std::exception& e)
			{
				response.setResponseCode(framework::ResponseCodes::internalServerError);
				response.setBody(e.what());
			}
		}
		else
		{
			response.setResponseCode(framework::ResponseCodes::notFound);
			response.setBody("Can't find user");
		}
	}

	void UsersExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table users = request.getTable(":memory:", "users");
		const framework::JSONParser& json = request.getJSON();
		std::string ownerUUID = json.get<std::string>("ownerUUID");
		std::string name = json.get<std::string>("userName");
		std::string newRole = json.get<std::string>("newRole");

		users.execute
		(
			"UPDATE users SET role = ? WHERE id = (SELECT id FROM users WHERE room_id = (SELECT room_id FROM users WHERE uuid = ?) AND name = ?)",
			{ framework::SQLValue(newRole), framework::SQLValue(ownerUUID), framework::SQLValue(name) }
		);

		framework::SQLResult sqlResult = users.execute
		(
			"SELECT uuid FROM rooms WHERE id = (SELECT room_id WHERE uuid = ?)",
			{ framework::SQLValue(ownerUUID) }
		);

		utils::notify<events::OnUpdateRoleEvent>(sqlResult.begin()->at("uuid").get<std::string>(), newRole);

		response.setBody("Role has been changed");
	}

	void UsersExecutor::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table users = request.getTable(":memory:", "users");
		const framework::JSONParser& json = request.getJSON();
		std::string uuid = json.get<std::string>("userUUId");
		std::string newUserName = json.get<std::string>("newUserName");
		std::string userName;
		std::string roomUUID;

		framework::SQLResult sqlResult = users.execute
		(
			"SELECT name, room_id FROM users WHERE uuid = ?",
			{ framework::SQLValue(uuid) }
		);

		users.execute
		(
			"UPDATE users SET name = ? WHERE uuid = ?",
			{ framework::SQLValue(newUserName), framework::SQLValue(uuid) }
		);

		userName = sqlResult.begin()->at("name").get<std::string>();

		sqlResult = users.execute
		(
			"SELECT uuid FROM rooms WHERE id = ?",
			{ framework::SQLValue(sqlResult.begin()->at("room_id").get<int64_t>()) }
		);

		utils::notify<events::OnUserNameUpdateEvent>(sqlResult.begin()->at("uuid").get<std::string>(), userName, newUserName);

		response.setBody("User name has been changed");
	}

	DEFINE_EXECUTOR(UsersExecutor);
}
