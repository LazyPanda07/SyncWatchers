#pragma once

#include "IEvent.h"

#include <string>

namespace events
{
	class OnUploadContentEvent : public IEvent
	{
	private:
		std::string uploadUserUUID;

	public:
		OnUploadContentEvent(const std::string& uploadUserUUID);

		EventId getEventId() const override;

		std::string_view getEventData() const override;

		virtual ~OnUploadContentEvent() = default;
	};
}
