#pragma once

#include <Executors/BaseStatelessExecutor.hpp>

namespace executors
{
	class StopExecutor : public framework::BaseStatelessExecutor
	{
	public:
		StopExecutor() = default;

		void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~StopExecutor() = default;
	};
}
