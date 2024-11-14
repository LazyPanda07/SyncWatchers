#include <iostream>

#include <import.h>

int main(int argc, char** argv)
{
    framework::utility::initializeWebFramework();

    framework::utility::Config config("configs/config.json");

    config.overrideBasePath("executors");

    framework::WebFramework server(config);
    int64_t port = config.getConfigurationInteger("port");

    server.start(true, [port]()
        {
            std::cout << "Server is running on port: " << port << std::endl;
        });

    return 0;
}
