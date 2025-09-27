#pragma once

#include "IEvent.h"

#include <string>

namespace events
{
	class PlayEvent : public IEvent
	{
	private:
		std::string playUserName;

	public:
		PlayEvent(std::string_view playUserName);

		EventId getEventId() const override;

		std::string_view getEventData() const override;

		~PlayEvent() = default;
	};
}
