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
		onRoomDelete,
		stop,
		play,
		rewind
	};
}
