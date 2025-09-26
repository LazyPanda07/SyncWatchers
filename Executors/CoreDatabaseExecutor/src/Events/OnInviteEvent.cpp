#include "OnInviteEvent.h"

#include "Utils.h"

namespace events
{
	OnInviteEvent::OnInviteEvent(std::string_view userName) :
		userName(utils::makeStringWithSize(userName))
	{
		
	}

	EventId events::OnInviteEvent::getEventId() const
	{
		return EventId::onInvite;
	}

	std::string_view events::OnInviteEvent::getEventData() const
	{
		return userName;
	}
}
