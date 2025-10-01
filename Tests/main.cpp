#include <format>
#include <chrono>
#include <thread>
#include <filesystem>

#include <gtest/gtest.h>
#include <curl/curl.h>

#include "utils/ProcessWrapper.h"

reproc::process runServer();

int main(int argc, char** argv)
{
	std::unique_ptr<process::ProcessWrapper> server;

	if (std::none_of(argv, argv + argc, [](const char* value) { using namespace std::string_view_literals; return value == "--external_server"sv; }))
	{
		server = std::make_unique<process::ProcessWrapper>(runServer());
	}

	testing::InitGoogleTest(&argc, argv);
	curl_global_init(CURL_GLOBAL_ALL);

	int result = RUN_ALL_TESTS();

	curl_global_cleanup();

	if (std::none_of(argv, argv + argc, [](const char* value) { using namespace std::string_view_literals; return value == "--keep_assets"sv; }))
	{
		std::filesystem::remove_all(std::filesystem::current_path() / "assets");
	}

	return result;
}

reproc::process runServer()
{
	reproc::process server;
	std::vector<std::string> args = { (std::filesystem::current_path() / "SyncWatchers").string(), "127.0.0.1" };
	reproc::options options;
	std::vector<uint8_t> buffer(256, '\0');

	if (std::error_code errorCode = server.start(reproc::arguments(args), options); errorCode)
	{
		std::cerr << errorCode.message() << std::endl;

		exit(errorCode.value());
	}

	while (true)
	{
		std::cout << "Waiting server..." << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		auto [numberOfBytes, errorCode] = server.read(reproc::stream::out, buffer.data(), buffer.size());

		if (errorCode)
		{
			std::cerr << errorCode.message() << std::endl;

			exit(errorCode.value());
		}

		if (numberOfBytes)
		{
			std::cout << buffer.data() << std::endl;

			break;
		}
	}

	return server;
}
