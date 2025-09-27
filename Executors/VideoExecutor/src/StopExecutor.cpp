#include "StopExecutor.h"

#include "Events/StopEvent.h"
#include "Utils.h"

namespace executors
{
	void StopExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		const framework::JSONParser& json = request.getJSON();

		utils::notify<events::StopEvent>(json.get<std::string>("room_uuid"), json.get<std::string>("userName"));

		response.setBody("Stop video");
	}

	DEFINE_EXECUTOR(StopExecutor);
}
