#pragma once

#include <Executors/BaseHeavyOperationStatefulExecutor.hpp>

#include <filesystem>

namespace executors
{
	class DownloadContentExecutor : public framework::BaseHeavyOperationStatefulExecutor
	{
	private:
		static int64_t getUtcTimeInSeconds(const std::filesystem::path& filePath);

	public:
		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
	};
}
