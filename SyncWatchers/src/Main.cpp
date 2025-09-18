#include <iostream>
#include <format>

#include <import.hpp>

int main(int argc, char** argv)
{
	framework::utility::initializeWebFramework();

	framework::utility::Config config("config.json");

	if (argc == 2)
	{
		config.overrideConfiguration("ip", argv[1]);
	}

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
