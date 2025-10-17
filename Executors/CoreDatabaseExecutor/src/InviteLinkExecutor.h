#pragma once

#include <Executors/BaseHeavyOperationStatelessExecutor.hpp>

namespace executors
{
	class InviteLinkExecutor : public framework::BaseHeavyOperationStatelessExecutor
	{
	private:
		static std::string generateDefaultName();

	private:
		bool useHTTPS;

	public:
		InviteLinkExecutor();

		void init(const framework::utility::ExecutorSettings& settings) override;

		void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~InviteLinkExecutor() = default;
	};
}
