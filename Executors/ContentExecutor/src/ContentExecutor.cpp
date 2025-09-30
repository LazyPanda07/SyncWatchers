#include "ContentExecutor.h"

namespace executors
{
	void ContentExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		try
		{
			framework::Table content = request.getTable(":memory:", "content");
			framework::SQLResult result = content.execute
			(
				"SELECT "
				"c.name   AS content_name,"
				"u.name   AS user_name"
				"FROM content c"
				"JOIN users u ON c.upload_user_id = u.id"
				"JOIN rooms r ON c.room_id = r.id"
				"WHERE r.uuid = ?",
				{ framework::SQLValue(request.getRouteParameter<std::string>("room_uuid")) }
			);

			printf("%d\n", result.size());
		}
		catch (const std::exception& e)
		{
			printf("%s\n", e.what());

			throw;
		}
	}

	DEFINE_EXECUTOR(ContentExecutor);
}
