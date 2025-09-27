#include "RewindEvent.h"

#include <cstring>

namespace events
{
	RewindEvent::RewindEvent(uint32_t startOffsetInSeconds) :
		startOffsetInSeconds(sizeof(startOffsetInSeconds), '\0')
	{
		std::memcpy(this->startOffsetInSeconds.data(), &startOffsetInSeconds, sizeof(startOffsetInSeconds));
	}

	EventId RewindEvent::getEventId() const
	{
		return EventId::rewind;
	}

	std::string_view RewindEvent::getEventData() const
	{
		return startOffsetInSeconds;
	}
}
