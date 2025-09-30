#include "ChangeVideoExecutor.h"

#include "Utils.h"
#include "Events/ChangeVideoEvent.h"

namespace executors
{
	void ChangeVideoExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		const framework::JSONParser& json = request.getJSON();

		utils::notify<events::ChangeVideoEvent>(json.get<std::string>("roomUUID"), json.get<std::string>("videoName"));

		response.setBody("Change video");
	}

	DEFINE_EXECUTOR(ChangeVideoExecutor);
}
