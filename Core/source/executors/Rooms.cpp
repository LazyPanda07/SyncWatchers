#include "Rooms.h"

#include <UUID.h>

#include "models/RoomModel.h"
#include "models/UserModel.h"

namespace executors
{
	void Rooms::init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void Rooms::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Rooms::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		std::shared_ptr<models::RoomModel> model = request.getModel<models::RoomModel>();
		std::string roomUUID = utility::generateUUID();
		std::string roomName = request.getJSON().getString("roomName");

		model->insert
		(
			{
				{ "uuid", roomUUID },
				{ "name", roomName }
			}
		);

		response.addBody(json::JSONBuilder(CP_UTF8).appendString("roomUUID", roomUUID));
	}

	void Rooms::doHead(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Rooms::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		std::shared_ptr<models::UserModel> model = request.getModel<models::UserModel>();
		const json::JSONParser& json = request.getJSON();
		std::string userUUID = utility::generateUUID();

		model->insert
		(
			{
				{ "uuid", userUUID },
				{ "name", json.getString("name") },
				{ "ip", request.getClientIpV4() },
				{ "room_uuid", json.getString("roomUUID") }
			}
		);

		response.addBody(json::JSONBuilder(CP_UTF8).appendString("uuid", userUUID));
	}

	void Rooms::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Rooms::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Rooms::doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		BaseStatelessExecutor::doOptions(request, response);
	}

	void Rooms::doTrace(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		BaseStatelessExecutor::doTrace(request, response);
	}

	void Rooms::doConnect(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	DECLARE_EXECUTOR(Rooms)
}
