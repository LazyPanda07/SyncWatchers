#include "PlayEvent.h"

#include "Utils.h"

namespace events
{
	PlayEvent::PlayEvent(std::string_view playUserName) :
		playUserName(utils::makeStringWithSize(playUserName))
	{

	}

	EventId PlayEvent::getEventId() const
	{
		return EventId::play;
	}

	std::string_view PlayEvent::getEventData() const
	{
		return playUserName;
	}
}
