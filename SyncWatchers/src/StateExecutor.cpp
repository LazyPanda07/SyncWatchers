#include "StateExecutor.h"

namespace executors
{
	void StateExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		response.setBody("Server is running...");
	}

	DEFINE_EXECUTOR(StateExecutor);
}
