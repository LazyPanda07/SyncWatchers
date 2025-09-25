#pragma once

#include "IEventsManager.h"

namespace utils
{
	events::IEventsManager& getEventsManager();

	std::string makeStringWithSize(std::string_view source);
}
