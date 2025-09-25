#include <iostream>
#include <format>
#include <filesystem>

#include <import.hpp>

#include "Utils.h"

int main(int argc, char** argv) try
{
	events::IEventsManager& events = utils::getEventsManager();

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
