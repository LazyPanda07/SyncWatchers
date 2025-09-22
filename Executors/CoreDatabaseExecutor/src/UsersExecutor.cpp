#include "UsersExecutor.h"

namespace executors
{
	void UsersExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table users = request.getTable(":memory:", "users");
		std::string uuid = request.getQueryParameters().at("user_uuid");
		framework::JSONBuilder result;

		framework::SQLResult sqlResult = users.execute
		(
			"SELECT id, name, role WHERE uuid = ?",
			{ framework::SQLValue(uuid) }
		);

		if (sqlResult.size())
		{
			const framework::SQLResult::Row& user = *sqlResult.begin();
			framework::Table content = request.getTable(":memory:", "content");
			std::vector<framework::JSONObject> uploadedContent;

			framework::SQLResult sqlContent = content.execute
			(
				"SELECT name FROM content WHERE uploaded_user_id = ?",
				{ framework::SQLValue(user.at("id").get<int64_t>()) }
			);

			for (const auto& row : sqlContent)
			{
				for (const auto& [_, contentName] : row)
				{
					framework::utility::appendArray(uploadedContent, contentName.get<std::string>());
				}
			}

			result["name"] = user.at("user").get<std::string>();
			result["role"] = user.at("role").get<std::string>();
			result["uploadedContent"] = uploadedContent;

			response.setBody(result);
		}
		else
		{
			response.setResponseCode(framework::ResponseCodes::notFound);

			response.setBody("Can't find user");
		}
	}

	void UsersExecutor::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table users = request.getTable(":memory:", "users");
		framework::JSONParser parser(request.getBody());
		std::string uuid = parser.get<std::string>("user_uuid");
		std::string newUserName = parser.get<std::string>("new_user_name");

		users.execute
		(
			"UPDATE users SET name = ? WHERE uuid = ?",
			{ framework::SQLValue(newUserName), framework::SQLValue(uuid) }
		);
	}

	DEFINE_EXECUTOR(UsersExecutor);
}
