#pragma once

#include <string>
#include <cstdint>

#ifdef __LINUX__
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

using SOCKET = int;
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

namespace tcp
{
	class Socket
	{
	private:
		SOCKET descriptor;

	public:
		Socket();

		void connect(std::string_view ip, int64_t port);

		void send(std::string_view data) const;

		void receive(std::string& data, size_t size = 0) const;

		template<typename T>
		void receive(T* data) const;

		~Socket();
	};

	template<typename T>
	void Socket::receive(T* data) const
	{
		recv(descriptor, reinterpret_cast<char*>(data), sizeof(T), NULL);
	}
}
