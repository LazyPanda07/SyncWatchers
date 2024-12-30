#include "NotificationsServer.h"

#include <fstream>

#include <JSONParser.h>

namespace notifications
{
	void NotificationsServer::clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr address, std::function<void()>& cleanup)
	{

	}

	NotificationsServer::NotificationsServer(std::string_view ip, int port, int timeout) :
		BaseTCPServer(std::to_string(port), ip, timeout)
	{

	}

	NotificationsServer& NotificationsServer::get()
	{
		static std::unique_ptr<NotificationsServer> server = nullptr;

		if (!server)
		{
			json::JSONParser parser(std::ifstream("configs/config.json"));
			const std::string& ip = parser.getString("ip", true);
			const json::utility::jsonObject& notificationsServerSettings = parser.getObject("NotificationsServer");

			int port = static_cast<int>(notificationsServerSettings.getInt("port"));
			int timeout = static_cast<int>(notificationsServerSettings.getInt("timeout"));

			server = std::unique_ptr<NotificationsServer>(new NotificationsServer(ip, port, timeout));
		}

		return *server;
	}
}
