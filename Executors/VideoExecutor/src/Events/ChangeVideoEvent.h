#pragma once

#include "IEvent.h"

#include <string>

namespace events
{
	class ChangeVideoEvent : public IEvent
	{
	private:
		std::string videoName;

	public:
		ChangeVideoEvent(std::string_view videoName);

		EventId getEventId() const override;

		std::string_view getEventData() const override;

		~ChangeVideoEvent() = default;
	};
}
