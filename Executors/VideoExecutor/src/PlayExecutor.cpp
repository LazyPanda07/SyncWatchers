#include "PlayExecutor.h"

#include "Utils.h"
#include "Events/PlayEvent.h"

namespace executors
{
	void PlayExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		const framework::JSONParser& json = request.getJSON();

		utils::notify<events::PlayEvent>(json.get<std::string>("roomUUID"), json.get<std::string>("userName"));

		response.setBody("Play video");
	}

	DEFINE_EXECUTOR(PlayExecutor);
}
