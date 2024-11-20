	#include "Users.h"

#include "models/UserToRoomModel.h"
#include "models/UserModel.h"

namespace executors
{
	void Users::init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void Users::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		std::shared_ptr<models::UserToRoomModel> userToRoomModel = request.getModel<models::UserToRoomModel>();
		std::shared_ptr<models::UserModel> userModel = request.getModel<models::UserModel>();
		const std::string& uuid = request.getRouteParameter<std::string>("uuid");
		std::vector<std::string> ids;
		std::vector<json::utility::jsonObject> users;

		framework::sqlite::utility::SQLiteResult result = userModel->selectByField("uuid", uuid);

		if (result.isEmpty())
		{
			response.setResponseCode(web::responseCodes::notFound);

			return;
		}

		std::string roomUUID = result[0].at("room_uuid");

		result = userToRoomModel->selectByField("room_uuid", roomUUID);

		if (result.isEmpty())
		{
			response.setResponseCode(web::responseCodes::notFound);

			return;
		}

		for (const std::unordered_map<std::string, std::string>& row : result)
		{
			ids.emplace_back(row.at("user_uuid"));
		}

		for (const std::string& userUUID : ids)
		{
			result = userModel->selectByField("uuid", userUUID);

			if (result.isEmpty())
			{
				continue;
			}

			json::utility::appendArray(result[0].at("name"), users);
		}

		response.addBody(json::JSONBuilder(CP_UTF8).appendArray("users", std::move(users)));
	}

	void Users::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Users::doHead(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Users::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		std::shared_ptr<models::UserToRoomModel> model = request.getModel<models::UserToRoomModel>();
		const std::string& uuid = request.getRouteParameter<std::string>("uuid");
		const std::string& roomUUID = request.getJSON().getString("roomUUID");

		model->insert
		(
			{
				{ "user_uuid", uuid },
				{ "room_uuid", roomUUID }
			}
		);
	}

	void Users::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		std::shared_ptr<models::UserToRoomModel> userToRoomModel = request.getModel<models::UserToRoomModel>();
		std::shared_ptr<models::UserModel> userModel = request.getModel<models::UserModel>();
		const std::string& uuid = request.getRouteParameter<std::string>("uuid");

		userToRoomModel->deleteQuery("user_uuid", uuid);
		userModel->deleteQuery("uuid", uuid);
	}

	void Users::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Users::doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		BaseStatelessExecutor::doOptions(request, response);
	}

	void Users::doTrace(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		BaseStatelessExecutor::doTrace(request, response);
	}

	void Users::doConnect(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	DECLARE_EXECUTOR(Users)
}
