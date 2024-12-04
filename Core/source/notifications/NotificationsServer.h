#pragma once

#include <BaseTCPServer.h>

namespace notifications
{
	class NotificationsServer : public web::BaseTCPServer
	{
	private:
		void clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr address, std::function<void()>&& cleanup) override;

	private:
		NotificationsServer(std::string_view ip, int port, int timeout);

	public:
		static NotificationsServer& get();
	};
}
