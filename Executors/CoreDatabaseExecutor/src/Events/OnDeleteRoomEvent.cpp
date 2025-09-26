#include "OnDeleteRoomEvent.h"

namespace events
{
	EventId OnDeleteRoomEvent::getEventId() const
	{
		return EventId::onRoomDelete;
	}

	std::string_view OnDeleteRoomEvent::getEventData() const
	{
		return std::string_view();
	}
}
