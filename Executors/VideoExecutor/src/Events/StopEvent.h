#pragma once

#include "IEvent.h"

#include <string>

namespace events
{
	class StopEvent : public IEvent
	{
	private:
		std::string stopUserName;

	public:
		StopEvent(std::string_view stopUserName);

		EventId getEventId() const override;

		std::string_view getEventData() const override;

		~StopEvent() = default;
	};
}
