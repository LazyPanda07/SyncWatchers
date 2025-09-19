#pragma once

#include <Executors/BaseHeavyOperationStatelessExecutor.hpp>

namespace executors
{
	class RoomsExecutor : public framework::BaseHeavyOperationStatelessExecutor
	{
	private:
		static std::string inviteLink;

	public:
		static std::string_view getBaseInviteLink();

	public:
		RoomsExecutor() = default;

		void init(const framework::utility::ExecutorSettings& settings) override;

		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~RoomsExecutor() = default;
	};
}
