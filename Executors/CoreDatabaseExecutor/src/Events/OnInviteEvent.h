#pragma once

#include "IEvent.h"

#include <string>

namespace events
{
	class OnInviteEvent : public IEvent
	{
	private:
		std::string userName;

	public:
		OnInviteEvent(const std::string& userName);

		EventId getEventId() const override;

		std::string_view getEventData() const override;

		~OnInviteEvent() = default;
	};
}
