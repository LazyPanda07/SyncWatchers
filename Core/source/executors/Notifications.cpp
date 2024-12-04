#include "Notifications.h"

namespace executors
{
	void Notifications::init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void Notifications::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Notifications::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Notifications::doHead(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Notifications::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Notifications::doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Notifications::doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	void Notifications::doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		BaseStatelessExecutor::doOptions(request, response);
	}

	void Notifications::doTrace(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		BaseStatelessExecutor::doTrace(request, response);
	}

	void Notifications::doConnect(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		throw framework::exceptions::NotImplementedException(typeid(*this).name(), __func__);
	}

	DECLARE_EXECUTOR(Notifications)
}
