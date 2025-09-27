#pragma once

#include <Executors/BaseStatelessExecutor.hpp>

namespace executors
{
	class RewindExecutor : public framework::BaseStatelessExecutor
	{
	public:
		RewindExecutor() = default;

		void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~RewindExecutor() = default;
	};
}
