#include "ChangeVideoEvent.h"

#include "Utils.h"

namespace events
{
	ChangeVideoEvent::ChangeVideoEvent(std::string_view videoName) :
		videoName(utils::makeStringWithSize(videoName))
	{

	}

	EventId ChangeVideoEvent::getEventId() const
	{
		return EventId::changeVideo;
	}
	
	std::string_view ChangeVideoEvent::getEventData() const
	{
		return videoName;
	}
}
