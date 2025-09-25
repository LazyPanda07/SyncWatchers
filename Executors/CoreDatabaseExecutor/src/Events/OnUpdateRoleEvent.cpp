#include "OnUpdateRoleEvent.h"

#include "Utils.h"

namespace events
{
	OnUpdateRoleEvent::OnUpdateRoleEvent(const std::string& role) :
		role(utils::makeStringWithSize(role))
	{

	}

	EventId events::OnUpdateRoleEvent::getEventId() const
	{
		return EventId::onUpdateRole;
	}

	std::string_view events::OnUpdateRoleEvent::getEventData() const
	{
		return role;
	}
}
