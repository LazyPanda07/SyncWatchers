#include <format>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <reproc++/run.hpp>

static size_t writeCallback(char* content, size_t size, size_t numberOfBytes, void* response)
{
	static_cast<std::string*>(response)->append(content, size * numberOfBytes);

	return size * numberOfBytes;
};

TEST(Rooms, CreateRoom)
{
	CURL* curl = curl_easy_init();
	std::string response;

	ASSERT_TRUE(curl);

	constexpr std::string_view jsonData = R"({"name": "NewRoom"})";

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:52000/rooms"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POST, 1L), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response), CURLE_OK);

	curl_slist* headers = nullptr;

	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(response);
	std::string inviteLink = responseJSON["inviteLink"].get<std::string>();

	response.clear();

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, inviteLink.data()), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POST, 0L), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	std::cout << response << std::endl;

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
}

TEST(Users, UpdateName)
{

}

std::error_code runServer(reproc::process& server);

int main(int argc, char** argv)
{
	reproc::process server;

	if (std::error_code code = runServer(server); code)
	{
		std::cerr << code.message() << std::endl;

		return 1;
	}

	std::this_thread::sleep_for(std::chrono::seconds(5)); // wait for server

	testing::InitGoogleTest(&argc, argv);
	curl_global_init(CURL_GLOBAL_ALL);

	int result = RUN_ALL_TESTS();

	curl_global_cleanup();

	server.kill();

	return result;
}

std::error_code runServer(reproc::process& server)
{
	std::vector<std::string> args = { (std::filesystem::current_path() / "SyncWatchers").string(), "127.0.0.1" };
	reproc::options options;

	return server.start(reproc::arguments(args), options);
}
