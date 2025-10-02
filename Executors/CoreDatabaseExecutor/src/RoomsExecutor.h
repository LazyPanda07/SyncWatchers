#pragma once

#include <Executors/BaseHeavyOperationStatelessExecutor.hpp>

namespace executors
{
	class RoomsExecutor : public framework::BaseHeavyOperationStatelessExecutor
	{
	public:
		RoomsExecutor() = default;

		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		void doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~RoomsExecutor() = default;
	};
}
