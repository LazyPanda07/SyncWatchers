#pragma once

#include <Executors/BaseHeavyOperationStatelessExecutor.hpp>

namespace executors
{
	class ContentExecutor : public framework::BaseHeavyOperationStatelessExecutor
	{
	public:
		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
	};
}
