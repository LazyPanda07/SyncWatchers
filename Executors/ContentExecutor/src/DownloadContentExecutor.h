#pragma once

#include <Executors/BaseHeavyOperationStatefulExecutor.hpp>

namespace executors
{
	class DownloadContentExecutor : public framework::BaseHeavyOperationStatefulExecutor
	{
	public:
		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
	};
}
