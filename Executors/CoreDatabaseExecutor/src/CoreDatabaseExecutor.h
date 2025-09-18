#pragma once

#include <Executors/BaseHeavyOperationStatefulExecutor.hpp>

namespace executors
{
	class CoreDatabaseExecutor : public framework::BaseHeavyOperationStatefulExecutor
	{
	public:
		CoreDatabaseExecutor() = default;

		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~CoreDatabaseExecutor() = default;
	};
}
