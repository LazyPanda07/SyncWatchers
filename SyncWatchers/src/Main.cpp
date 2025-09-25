#include <iostream>
#include <format>
#include <filesystem>

#include <import.hpp>

#include "IEventsManager.h"

#ifdef __LINUX__
using HMODULE = void*;
#endif

HMODULE loadLibrary(std::string_view libraryPath)
{
#ifdef __LINUX__
	return dlopen(libraryPath.data(), RTLD_LAZY);
#else
	return LoadLibraryA(libraryPath.data());
#endif
}

template<typename T>
T load(HMODULE handle, std::string_view name)
{
#ifdef __LINUX__
	return reinterpret_cast<T>(dlsym(handle, name.data()));
#else
	return reinterpret_cast<T>(GetProcAddress(handle, name.data()));
#endif
};

int main(int argc, char** argv) try
{
#ifdef __LINUX__
	HMODULE module = loadLibrary("libEvents.so");
#else
	HMODULE module = loadLibrary("Events.dll");
#endif
	
	auto getter = load<events::IEventsManager * (*)()>(module, "getEventsManager");

	events::IEventsManager& events = *getter();

	framework::utility::initializeWebFramework();

	framework::utility::Config config("config.json");

	if (argc == 2)
	{
		config.overrideConfiguration("ip", argv[1]);
	}

	events.startServer(config.getConfigurationString("ip"), config.getConfigurationInteger("port"));

	framework::WebFramework server(config);

	server.start
	(
		true,
		[&config]()
		{
			std::string protocol = config.getConfigurationBoolean("useHTTPS") ? "https" : "http";

			std::cout << std::format("Server is running at {}://{}:{}", protocol, config.getConfigurationString("ip"), config.getConfigurationInteger("port")) << std::endl;
		}
	);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	return 1;
}
