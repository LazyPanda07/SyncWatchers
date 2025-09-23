#include <fstream>
#include <filesystem>
#include <format>

#include <curl/curl.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

class BasicFunctionality : public testing::Test
{
private:
	static size_t writeCallback(char* content, size_t size, size_t numberOfBytes, void* response)
	{
		static_cast<std::string*>(response)->append(content, size * numberOfBytes);

		return size * numberOfBytes;
	};

protected:
	static constexpr std::string_view newUserName = "host";

protected:
	inline static std::string inviteLink;
	inline static std::string userName;
	inline static std::string userUUID;
	inline static std::string roomUUID;
	inline static std::string role;

protected:
	CURL* curl;
	std::string response;

public:
	BasicFunctionality() :
		curl(nullptr)
	{

	}

	void SetUp() override
	{
		curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &BasicFunctionality::writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response), CURLE_OK;
	}

	void TearDown()
	{
		curl_easy_cleanup(curl);
	}
};

TEST_F(BasicFunctionality, CreateRoom)
{
	constexpr std::string_view jsonData = R"({"name": "NewRoom"})";

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:52000/rooms"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POST, 1L), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	curl_slist* headers = nullptr;

	headers = curl_slist_append(headers, "Content-Type: application/json");

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(BasicFunctionality::response);

	BasicFunctionality::inviteLink = responseJSON["inviteLink"].get<std::string>();

	curl_slist_free_all(headers);
}

TEST_F(BasicFunctionality, JoinRoom)
{
	constexpr std::string_view jsonData = R"({"role": "owner"})";

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, inviteLink.data()), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(BasicFunctionality::response);

	BasicFunctionality::userName = responseJSON["userName"].get<std::string>();
	BasicFunctionality::userUUID = responseJSON["userUUID"].get<std::string>();
	BasicFunctionality::roomUUID = responseJSON["roomUUID"].get<std::string>();
	BasicFunctionality::role = responseJSON["role"].get<std::string>();
}

TEST_F(BasicFunctionality, UpdateName)
{
	std::string jsonData = std::format(R"({{"userUUId": "{}", "newUserName": "{}"}})", BasicFunctionality::userUUID, BasicFunctionality::newUserName);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:52000/users"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	curl_slist* headers = nullptr;

	curl_slist_append(headers, "Content-Type: application/json");

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	curl_slist_free_all(headers);
}

TEST_F(BasicFunctionality, GetRoomInformation)
{
	std::string url = std::format("http://127.0.0.1:52000/users?user_uuid={}", BasicFunctionality::userUUID);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(BasicFunctionality::response);

	ASSERT_EQ(responseJSON["name"].get<std::string>(), BasicFunctionality::newUserName);
	ASSERT_EQ(responseJSON["role"].get<std::string>(), "owner");
}

TEST_F(BasicFunctionality, UploadContent)
{
	std::string url = std::format("http://127.0.0.1:52000/upload/{}/{}/{}", BasicFunctionality::roomUUID, BasicFunctionality::userUUID, "content.mp4");
	FILE* file = fopen("content.mp4", "rb");
	
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_READDATA, file), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, std::filesystem::file_size("content.mp4")), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	ASSERT_EQ(BasicFunctionality::response, "File uploaded");
}

TEST_F(BasicFunctionality, AvailableContent)
{
	std::string url = std::format("http://127.0.0.1:52000/users?user_uuid={}", BasicFunctionality::userUUID);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(BasicFunctionality::response);
	const auto& content = responseJSON["uploadedContent"];

	ASSERT_EQ(content.size(), 1);
	ASSERT_EQ(content[0].get<std::string>(), "content.mp4");
}

TEST_F(BasicFunctionality, DownloadContent)
{
	std::string url = std::format("http://127.0.0.1:52000/download/{}/content.mp4", BasicFunctionality::roomUUID);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	std::cout << BasicFunctionality::response.size() << std::endl;
}

TEST_F(BasicFunctionality, DeleteRoom)
{
	std::string jsonData = std::format(R"({{"room_uuid": "{}", }})", BasicFunctionality::roomUUID);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:52000/rooms"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	curl_slist* headers = nullptr;

	headers = curl_slist_append(headers, "Content-Type: application/json");

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	curl_slist_free_all(headers);

	ASSERT_EQ(BasicFunctionality::response, "Room deleted");
}
