#pragma once

#include <string_view>

#include "EventId.h"

namespace events
{
	class IEvent
	{
	public:
		virtual EventId getEventId() const = 0;

		virtual std::string_view getEventData() const = 0;

		virtual ~IEvent() = default;
	};
}
