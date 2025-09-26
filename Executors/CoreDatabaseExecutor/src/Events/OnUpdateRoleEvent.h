#pragma once

#include "IEvent.h"

#include <string>

namespace events
{
	class OnUpdateRoleEvent : public IEvent
	{
	private:
		std::string role;

	public:
		OnUpdateRoleEvent(std::string_view role);

		EventId getEventId() const override;

		std::string_view getEventData() const override;

		~OnUpdateRoleEvent() = default;
	};
}
