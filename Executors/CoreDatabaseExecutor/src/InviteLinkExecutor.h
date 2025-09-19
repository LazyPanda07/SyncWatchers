#pragma once

#include <Executors/BaseHeavyOperationStatelessExecutor.hpp>

namespace executors
{
	class InviteLinkExecutor : public framework::BaseHeavyOperationStatelessExecutor
	{
	private:
		static std::string generateDefaultName();

	public:
		void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
	};
}
