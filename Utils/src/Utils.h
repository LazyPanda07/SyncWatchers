#pragma once

#include "IEventsManager.h"

#include <concepts>
#include <string>

namespace utils
{
	events::IEventsManager& getEventsManager();

	template<typename... Args>
	std::string makeStringWithSize(Args... args) requires (std::same_as<Args, std::string_view> && ...);
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
}
