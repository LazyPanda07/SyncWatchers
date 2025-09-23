#include "DownloadContentExecutor.h"

#include <fstream>
#include <format>

namespace executors
{
	int64_t DownloadContentExecutor::getUtcTimeInSeconds(const std::filesystem::path& filePath)
	{
		auto time = std::chrono::utc_clock::from_sys(std::chrono::clock_cast<std::chrono::system_clock>(std::filesystem::last_write_time(filePath)));

		return std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count();
	}

	void DownloadContentExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		std::string uuid = request.getRouteParameter<std::string>("room_uuid");
		std::string fileName = request.getRouteParameter<std::string>("file_name");
		std::filesystem::path file(std::filesystem::current_path() / "assets" / uuid / fileName);

		if (!std::filesystem::exists(file))
		{
			response.setResponseCode(framework::ResponseCodes::notFound);
			response.setBody(std::format("Can't find {}", fileName));

			return;
		}

		request.streamFile((std::filesystem::path(uuid) / fileName).string(), response, fileName);
	}

	void DownloadContentExecutor::doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		std::string uuid = request.getRouteParameter<std::string>("room_uuid");
		std::string fileName = request.getRouteParameter<std::string>("file_name");
		std::filesystem::path file(std::filesystem::current_path() / "assets" / uuid / fileName);

		if (!std::filesystem::exists(file))
		{
			response.setResponseCode(framework::ResponseCodes::notFound);
			response.setBody(std::format("Can't find {}", fileName));

			return;
		}

		framework::JSONBuilder result;

		result["fileSize"] = std::filesystem::file_size(file);
		result["lastModifiedTime"] = DownloadContentExecutor::getUtcTimeInSeconds(file);

		response.setBody(result);
	}

	DEFINE_EXECUTOR(DownloadContentExecutor);
}
