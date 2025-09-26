#pragma once

#include "IEvent.h"

#include <string>

namespace events
{
	class OnUserNameUpdateEvent : public IEvent
	{
	private:
		std::string userNames;

	public:
		OnUserNameUpdateEvent(std::string_view oldUserName, std::string_view newUserName);

		EventId getEventId() const override;

		std::string_view getEventData() const override;

		~OnUserNameUpdateEvent() = default;
	};
}
