#pragma once

#include <cstdint>

namespace events
{
	enum class EventId : uint8_t
	{
		onUploadContent,
		onInvite,
		onUpdateRole,
		onUserNameUpdate,
		onDeleteRoom,
		stop,
		play,
		rewind,
		changeVideo
	};
}
