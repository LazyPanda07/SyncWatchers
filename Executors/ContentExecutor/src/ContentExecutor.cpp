#include "ContentExecutor.h"

namespace executors
{
	void ContentExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		framework::Table content = request.getTable(":memory:", "content");
		framework::SQLResult sqlResult = content.execute
		(
			"SELECT "
			"content.name AS content_name, "
			"users.name AS user_name "
			"FROM content "
			"JOIN users ON content.upload_user_id = users.id "
			"JOIN rooms ON content.room_id = rooms.id "
			"WHERE rooms.uuid = ?",
			{ framework::SQLValue(request.getRouteParameter<std::string>("room_uuid")) }
		);
		std::vector<framework::JSONObject> result;

		for (const framework::SQLResult::Row& row : sqlResult)
		{
			framework::JSONObject object;

			object.setValue("contentName", row.at("content_name").get<std::string>());
			object.setValue("userName", row.at("user_name").get<std::string>());

			framework::utility::appendArray(result, object);
		}

		response.setBody(framework::JSONBuilder().append("uploadedContent", result));
	}

	DEFINE_EXECUTOR(ContentExecutor);
}
