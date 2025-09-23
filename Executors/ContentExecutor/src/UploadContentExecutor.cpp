#include "UploadContentExecutor.h"

#include <filesystem>
#include <format>

namespace executors
{
	void UploadContentExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		if (!stream.is_open())
		{
			framework::Table users = request.getTable(":memory:", "users");

			framework::SQLResult sqlResult = users.execute
			(
				"SELECT role FROM users WHERE uuid = ?",
				{ framework::SQLValue(request.getRouteParameter<std::string>("user_uuid")) }
			);

			if (sqlResult.begin()->at("role").get<std::string>() == "default")
			{
				response.setIsValid(true);
				response.setResponseCode(framework::ResponseCodes::forbidden);
				response.setBody("User can't upload content");

				return;
			}

			std::filesystem::path baseAssetsPath = std::filesystem::current_path() / "assets";

			std::string directoryName = request.getRouteParameter<std::string>("room_uuid");

			if (!std::filesystem::exists(baseAssetsPath / directoryName))
			{
				std::filesystem::create_directories(baseAssetsPath / directoryName);
			}

			stream.open(baseAssetsPath / directoryName / request.getRouteParameter<std::string>("file_name"), std::ios::binary);
		}

		std::string_view data = request.getBody();
		bool last = false;

		if (data.size())
		{
			last = true;
		}
		else
		{
			data = request.getLargeData().dataPart;
			last = request.getLargeData().isLastPacket;
		}

		stream.write(data.data(), data.size());

		if (last)
		{
			stream.close();

			framework::Table content = request.getTable(":memory:", "content");

			content.execute
			(
				"INSERT INTO content (name, room_id, upload_user_id) VALUES (?, (SELECT id FROM rooms WHERE uuid = ?), (SELECT id FROM users WHERE uuid = ?))",
				{ framework::SQLValue(request.getRouteParameter<std::string>("file_name")), framework::SQLValue(request.getRouteParameter<std::string>("room_uuid")), framework::SQLValue(request.getRouteParameter<std::string>("user_uuid")) }
			);

			response.setBody("File uploaded");
		}
	}

	DEFINE_EXECUTOR(UploadContentExecutor);
}
