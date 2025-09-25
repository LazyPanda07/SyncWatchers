#include "Socket.h"

#include <stdexcept>

#ifndef __LINUX__
#pragma comment(lib, "ws2_32.lib")
#endif

namespace tcp
{
	Socket::Socket() :
		descriptor(INVALID_SOCKET)
	{
#ifndef __LINUX__
		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			throw std::runtime_error(std::to_string(__LINE__));
		}
#endif // !__LINUX__
	}

	void Socket::connect(std::string_view ip, int64_t port)
	{
		addrinfo* info = nullptr;
		addrinfo hints = {};

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		if (getaddrinfo(ip.data(), std::to_string(port).data(), &hints, &info))
		{
			throw std::runtime_error(std::to_string(__LINE__));
		}

		if (descriptor = socket(info->ai_family, info->ai_socktype, info->ai_protocol); descriptor == INVALID_SOCKET)
		{
			freeaddrinfo(info);

			throw std::runtime_error(std::to_string(__LINE__));
		}

		if (::connect(descriptor, info->ai_addr, static_cast<int>(info->ai_addrlen)) == SOCKET_ERROR)
		{
			freeaddrinfo(info);

			throw std::runtime_error(std::to_string(__LINE__));
		}

		freeaddrinfo(info);
	}

	void Socket::send(std::string_view data) const
	{
		::send(descriptor, data.data(), data.size(), 0);
	}

	void Socket::receive(std::string& data, size_t size) const
	{
		size_t realSize = size ? size : data.size();
		size_t currentSize = 0;

		while (realSize != currentSize)
		{
			int received = recv(descriptor, data.data() + currentSize, realSize - currentSize, 0);

			currentSize += received;
		}
	}

	Socket::~Socket()
	{
		closesocket(descriptor);
	}
}
