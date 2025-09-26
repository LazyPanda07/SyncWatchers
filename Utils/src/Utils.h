#pragma once

#include <concepts>
#include <string>
#include <cstring>

#include "IEventsManager.h"

namespace utils
{
	events::IEventsManager& getEventsManager();

	template<typename... Args>
	std::string makeStringWithSize(Args... args) requires (std::same_as<Args, std::string_view> && ...);

	template<std::derived_from<events::IEvent> EventT, typename... Args>
	void notify(const std::string& roomUUID, Args&&... args);
}

namespace utils
{
	template<typename... Args>
	inline std::string makeStringWithSize(Args... args) requires (std::same_as<Args, std::string_view> && ...)
	{
		int size = (static_cast<int>(args.size()) + ...) + sizeof...(Args) - 1;
		std::string result(size + sizeof(size), ' ');
		size_t currentOffset = sizeof(size);
		auto callback = [&result, &currentOffset](std::string_view value)
			{
				std::memcpy(result.data() + currentOffset, value.data(), value.size());

				currentOffset += value.size() + 1; // ' '
			};

		std::memcpy(result.data(), &size, sizeof(size));

		(callback(args), ...);

		return result;
	}

	template<std::derived_from<events::IEvent> EventT, typename... Args>
	inline void notify(const std::string& roomUUID, Args&&... args)
	{
		getEventsManager().notify(EventT(std::forward<Args>(args)...), roomUUID);
	}
}
