#include "OnUploadContentEvent.h"

namespace events
{
	OnUploadContentEvent::OnUploadContentEvent(std::string_view uploadUserUUID) :
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
