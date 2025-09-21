#pragma once

#include <Executors/BaseStatelessExecutor.hpp>

namespace executors
{
	class StateExecutor : public framework::BaseStatelessExecutor
	{
	public:
		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
	};
}
