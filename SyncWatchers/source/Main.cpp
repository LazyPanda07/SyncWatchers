#include <iostream>
#include <format>

#include <import.h>

int main(int argc, char** argv)
{
#ifdef REMOVE_DATABASE_ON_START
    std::remove("core.sqlite");
#endif

    framework::utility::initializeWebFramework();

    framework::utility::Config config("configs/config.json");

    config.overrideBasePath("executors");

    framework::WebFramework server(config);
    int64_t port = config.getConfigurationInteger("port");

    server.start(true, [port]()
        {
            std::cout << std::format("Start WebFramework server: {}", framework::WebFramework::getWebFrameworkVersion()) << std::endl;
            std::cout << std::format("Server is running on port: {}", port) << std::endl;
        });

    return 0;
}
