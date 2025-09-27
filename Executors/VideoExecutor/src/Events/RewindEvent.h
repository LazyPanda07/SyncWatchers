#pragma once

#include "IEvent.h"

#include <string>

namespace events
{
	class RewindEvent : public IEvent
	{
	private:
		std::string startOffsetInSeconds;

	public:
		RewindEvent(uint32_t startOffsetInSeconds);

		EventId getEventId() const override;

		std::string_view getEventData() const override;

		~RewindEvent() = default;
	};
}
