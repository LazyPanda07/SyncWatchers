#include "OnUserNameUpdateEvent.h"

#include "Utils.h"

namespace events
{
	OnUserNameUpdateEvent::OnUserNameUpdateEvent(std::string_view oldUserName, std::string_view newUserName) :
		userNames(utils::makeStringWithSize(oldUserName, newUserName))
	{
		
	}

	EventId events::OnUserNameUpdateEvent::getEventId() const
	{
		return EventId::onUserNameUpdate;
	}

	std::string_view events::OnUserNameUpdateEvent::getEventData() const
	{
		return userNames;
	}
}
