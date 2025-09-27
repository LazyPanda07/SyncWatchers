#include "StopEvent.h"

#include "Utils.h"

namespace events
{
	StopEvent::StopEvent(std::string_view stopUserName) :
		stopUserName(utils::makeStringWithSize(stopUserName))
	{

	}

	EventId StopEvent::getEventId() const
	{
		return EventId::stop;
	}

	std::string_view StopEvent::getEventData() const
	{
		return stopUserName;
	}
}
