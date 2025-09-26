#pragma once

#include "IEvent.h"

namespace events
{
	class OnDeleteRoomEvent : public IEvent
	{
	public:
		OnDeleteRoomEvent() = default;

		EventId getEventId() const override;

		std::string_view getEventData() const override;

		~OnDeleteRoomEvent() = default;
	};
}
