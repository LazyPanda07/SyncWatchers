#pragma once

#include <Executors/BaseHeavyOperationStatefulExecutor.hpp>

#include <fstream>

namespace executors
{
	class UploadContentExecutor : public framework::BaseHeavyOperationStatefulExecutor
	{
	private:
		std::ofstream stream;

	public:
		void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
	};
}
