#include "RewindExecutor.h"

#include "Utils.h"
#include "Events/RewindEvent.h"

namespace executors
{
	void RewindExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		const framework::JSONParser& json = request.getJSON();

		utils::notify<events::RewindEvent>(json.get<std::string>("room_uuid"), json.get<uint32_t>("offset"));

		response.setBody("Rewind video");
	}
}
