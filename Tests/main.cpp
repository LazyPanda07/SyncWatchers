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

class Database : public testing::Test
{
protected:
	CURL* curl;
	std::string userName;
	std::string userUUID;
	std::string role;

public:
	Database() :
		curl(nullptr)
	{
		
	}

	void SetUp() override
	{
		curl = curl_easy_init();
	}

	void TearDown()
	{
		curl_easy_cleanup(curl);
	}
};

TEST_F(Database, CreateRoom)
{
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

	responseJSON = nlohmann::json::parse(response);

	userName = responseJSON["userName"].get<std::string>();
	userUUID = responseJSON["userUUID"].get<std::string>();
	role = responseJSON["role"].get<std::string>();

	curl_slist_free_all(headers);
}

TEST_F(Database, UpdateName)
{

}

reproc::process runServer();

int main(int argc, char** argv)
{
	reproc::process server = runServer();

	testing::InitGoogleTest(&argc, argv);
	curl_global_init(CURL_GLOBAL_ALL);

	int result = RUN_ALL_TESTS();

	curl_global_cleanup();

	server.kill();

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
		std::this_thread::sleep_for(std::chrono::seconds(1));

		auto [numberOfBytes, errorCode] = server.read(reproc::stream::out, buffer.data(), buffer.size());

		if (errorCode)
		{
			std::cerr << errorCode.message() << std::endl;

			exit(errorCode.value());
		}

		if (numberOfBytes)
		{
			break;
		}
	}

	return server;
}
