#pragma once

#include <Executors/BaseStatelessExecutor.hpp>

namespace executors
{
	class ChangeVideoExecutor : public framework::BaseStatelessExecutor
	{
	public:
		ChangeVideoExecutor() = default;

		void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~ChangeVideoExecutor() = default;
	};
}
