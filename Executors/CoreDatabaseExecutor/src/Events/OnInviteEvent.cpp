#include "OnInviteEvent.h"

#include "Utils.h"

namespace events
{
	OnInviteEvent::OnInviteEvent(const std::string& userName) :
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
