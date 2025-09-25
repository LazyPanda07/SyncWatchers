#include "OnUploadContentEvent.h"

namespace events
{
	OnUploadContentEvent::OnUploadContentEvent(const std::string& uploadUserUUID) :
		uploadUserUUID(uploadUserUUID)
	{

	}

	EventId OnUploadContentEvent::getEventId() const
	{
		return EventId::onUploadContent;
	}

	std::string_view OnUploadContentEvent::getEventData() const
	{
		return uploadUserUUID;
	}
}
