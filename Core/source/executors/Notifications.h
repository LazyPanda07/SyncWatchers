#pragma once

#include <Executors/BaseStatelessExecutor.h>

namespace executors
{
	class Notifications : public framework::BaseStatelessExecutor
	{
	private:
		const std::unordered_map<std::string, std::function<void(const std::string& userUuid, const std::string& roomUuid, framework::HTTPRequest& request)>> actions;

	private:
		static void start(const std::string& userUuid, const std::string& roomUuid, framework::HTTPRequest& request);

		static void stop(const std::string& userUuid, const std::string& roomUuid, framework::HTTPRequest& request);

		static void rewind(const std::string& userUuid, const std::string& roomUuid, framework::HTTPRequest& request);

		static void sync(const std::string& userUuid, const std::string& roomUuid, framework::HTTPRequest& request);

	public:
		Notifications();

		void init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings) override;

		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doHead(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doTrace(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doConnect(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~Notifications() = default;
	};
}
