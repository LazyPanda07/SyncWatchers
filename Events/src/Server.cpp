#include "Server.h"

#include <stdexcept>

#ifdef __LINUX__
#include <fcntl.h>
#include <arpa/inet.h>
#endif

#include "EventsManager.h"

#ifndef __LINUX__
#pragma comment (lib, "ws2_32.lib")
#endif

namespace events
{
	void Server::createListenSocket()
	{
		addrinfo* info = nullptr;
		addrinfo hints = {};

		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_socktype = SOCK_STREAM;

		if (getaddrinfo(ip.data(), std::to_string(port).data(), &hints, &info))
		{
			throw std::runtime_error(std::to_string(__LINE__));
		}

		if ((listenSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) == INVALID_SOCKET)
		{
			freeaddrinfo(info);

			throw std::runtime_error(std::to_string(__LINE__));
		}

		int yes = 1;

		if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&yes), sizeof(yes)) == SOCKET_ERROR)
		{
			freeaddrinfo(info);

			throw std::runtime_error(std::to_string(__LINE__));
		}

		if (bind(listenSocket, info->ai_addr, static_cast<int>(info->ai_addrlen)) == SOCKET_ERROR)
		{
			freeaddrinfo(info);

			throw std::runtime_error(std::to_string(__LINE__));
		}

		if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			freeaddrinfo(info);

			throw std::runtime_error(std::to_string(__LINE__));
		}

		freeaddrinfo(info);
	}

	void Server::receiveConnections()
	{
#ifdef __LINUX__
		socklen_t addrlen = sizeof(sockaddr);
#else
		int addrlen = sizeof(sockaddr);
#endif
		EventsManager& instance = EventsManager::getInstance();

		while (isRunning)
		{
			sockaddr address;
			SOCKET clientSocket = accept(listenSocket, &address, &addrlen);

			if (clientSocket != INVALID_SOCKET)
			{
				instance.addSocket(clientSocket);
			}
		}
	}

	Server::Server(std::string_view ip, int64_t port) :
		ip(ip),
		port(port),
		isRunning(false)
	{
#ifndef __LINUX__
		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			throw std::runtime_error(std::to_string(__LINE__));
		}
#endif // __LINUX__

		this->createListenSocket();
	}

	void Server::start()
	{
		isRunning = true;

		waiter = std::async(std::launch::async, &Server::receiveConnections, this);
	}

	void Server::stop()
	{
		isRunning = false;

		waiter.wait();

		closesocket(listenSocket);
	}

	Server::~Server()
	{
		this->stop();
	}
}
