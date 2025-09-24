#pragma once

#include <string>
#include <future>

#ifdef __LINUX__
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif // __LINUX__

#ifdef __LINUX__
#ifndef WINDOWS_STYLE_DEFINITION
#define WINDOWS_STYLE_DEFINITION

#define closesocket close
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define DWORD uint32_t

#endif // WINDOWS_STYLE_DEFINITION
#endif // __LINUX__

namespace events
{
	class Server
	{
	private:
		std::future<void> waiter;
		std::string ip;
		int64_t port;
		SOCKET listenSocket;
		bool isRunning;

	private:
		void createListenSocket();

		void receiveConnections();

	public:
		Server(std::string_view ip, int64_t port);

		void start();

		void stop();

		~Server();
	};
}
