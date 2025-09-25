#pragma once

#include <string_view>
#include <cstdint>

namespace events
{
	class IEventsManager
	{
	public:
		virtual void startServer(std::string_view ip, int64_t port) = 0;

		virtual size_t getListeners() const = 0;

		virtual ~IEventsManager() = default;
	};
}
