#include "Utils.h"

#ifdef __LINUX__
#include <dlfcn.h>
#else
#include <Windows.h>
#endif

namespace utils
{
	events::IEventsManager& getEventsManager()
	{
#ifdef __LINUX__
		static void* eventsLibrary = dlopen("libEvents.so", RTLD_LAZY);

		return *reinterpret_cast<events::IEventsManager * (*)()>(dlsym(eventsLibrary, "getEventsManager"))();
#else
		static HMODULE eventsLibrary = LoadLibrary(TEXT("Events.dll"));

		return *reinterpret_cast<events::IEventsManager * (*)()>(GetProcAddress(eventsLibrary, "getEventsManager"))();
#endif
	}

	std::string makeStringWithSize(std::string_view source)
	{
		int size = static_cast<int>(source.size());
		std::string	result(sizeof(size) + source.size(), '\0');

		std::memcpy(result.data(), &size, sizeof(size));
		std::memcpy(result.data() + sizeof(size), result.data(), result.size());

		return result;
	}
}
