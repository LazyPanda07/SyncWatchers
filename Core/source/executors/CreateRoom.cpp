#include "CreateRoom.h"

#include <UUID.h>

void CreateRoom::init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings)
{

}

void CreateRoom::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void CreateRoom::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	std::string roomUUID = utility::generateUUID();


}

void CreateRoom::doHead(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void CreateRoom::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void CreateRoom::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void CreateRoom::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

void CreateRoom::doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	BaseStatelessExecutor::doOptions(request, response);
}

void CreateRoom::doTrace(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	BaseStatelessExecutor::doTrace(request, response);
}

void CreateRoom::doConnect(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
}

DECLARE_EXECUTOR(CreateRoom)
