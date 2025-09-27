#pragma once

#include <Executors/BaseStatelessExecutor.hpp>

namespace executors
{
	class PlayExecutor : public framework::BaseStatelessExecutor
	{
	public:
		PlayExecutor() = default;

		void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~PlayExecutor() = default;
	};
}
