#pragma once

#include <Executors/BaseHeavyOperationStatelessExecutor.hpp>

namespace executors
{
	class UsersExecutor : public framework::BaseHeavyOperationStatelessExecutor
	{
	public:
		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
	};
}
