#include "StateExecutor.h"

namespace executors
{
	void StateExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		try
		{
			framework::Table rooms = request.getTable(":memory:", "rooms");
			framework::Table users = request.getTable(":memory:", "users");
			framework::JSONBuilder result;

			framework::SQLResult roomsSql = rooms.execute
			(
				"SELECT COUNT(*) AS rooms_count FROM rooms",
				{}
			);

			framework::SQLResult usersSql = rooms.execute
			(
				"SELECT COUNT(*) AS users_count FROM users",
				{}
			);

			result["numberOfRooms"] = roomsSql.begin()->at("rooms_count").get<int64_t>();
			result["numberOfUsers"] = usersSql.begin()->at("users_count").get<int64_t>();

			response.setBody(result);
		}
		catch (const std::exception&)
		{
			response.setBody("No rooms");
		}
	}

	DEFINE_EXECUTOR(StateExecutor);
}
