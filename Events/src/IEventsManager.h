#pragma once

#include <string>

#include "IEvent.h"

namespace events
{
	class IEventsManager
	{
	public:
		virtual void startServer(std::string_view ip, int64_t port) = 0;

		virtual void notify(const IEvent& event, const std::string& roomUUID) = 0;

		virtual size_t getListeners() const = 0;

		virtual ~IEventsManager() = default;
	};
}
