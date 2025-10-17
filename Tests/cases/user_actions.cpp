#include <fstream>
#include <filesystem>
#include <format>
#include <sstream>

#include <curl/curl.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "utils/Socket.h"

#define ASSERT_RESPONSE_CODE(code) long responseCode; curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode); ASSERT_EQ(code, responseCode)

class UserActions : public testing::Test
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
	static constexpr std::array<std::string_view, 2> getContentFiles()
	{
		return
		{
			"content_10.mp4",
			"content_110.mp4"
		};
	}

	static size_t writeFileCallback(char* content, size_t size, size_t numberOfBytes, void* file)
	{
		static_cast<std::ofstream*>(file)->write(content, size * numberOfBytes);

		return size * numberOfBytes;
	}

	static bool compareFiles(const std::filesystem::path& first, const std::filesystem::path& second)
	{
		if (!std::filesystem::exists(first) || !std::filesystem::exists(second))
		{
			return false;
		}

		if (std::filesystem::file_size(first) != std::filesystem::file_size(second))
		{
			return false;
		}

		std::ifstream inFirst(first, std::ios::binary);
		std::ifstream inSecond(second, std::ios::binary);

		constexpr size_t chunkSize = 1LL * 1024 * 1024;
		std::string firstData(chunkSize, '\0');
		std::string secondData(chunkSize, '\0');

		for (size_t i = 0; i < std::filesystem::file_size(first); i += chunkSize)
		{
			inFirst.read(firstData.data(), firstData.size());
			inSecond.read(secondData.data(), secondData.size());

			if (firstData != secondData)
			{
				return false;
			}
		}

		return true;
	}

protected:
	inline static std::string inviteLink;
	inline static std::string userName;
	inline static std::string userUUID;
	inline static std::string roomUUID;
	inline static std::string role;
	inline static tcp::Socket socket;

protected:
	CURL* curl;
	std::string response;

public:
	UserActions() :
		curl(nullptr)
	{

	}

	void SetUp() override
	{
		curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &UserActions::writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response), CURLE_OK;
	}

	void TearDown()
	{
		curl_easy_cleanup(curl);
	}
};

TEST_F(UserActions, CreateRoom)
{
	constexpr std::string_view jsonData = R"({"name": "NewRoom"})";

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:52000/rooms"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POST, 1L), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	ASSERT_RESPONSE_CODE(201);

	auto responseJSON = nlohmann::json::parse(UserActions::response);

	UserActions::inviteLink = responseJSON["inviteLink"].get<std::string>();

	curl_slist_free_all(headers);
}

TEST_F(UserActions, JoinRoom)
{
	constexpr std::string_view jsonData = R"({"role": "owner"})";

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, inviteLink.data()), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POST, 1L), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	ASSERT_RESPONSE_CODE(201);

	auto responseJSON = nlohmann::json::parse(UserActions::response);

	UserActions::userName = responseJSON["userName"].get<std::string>();
	UserActions::userUUID = responseJSON["userUUID"].get<std::string>();
	UserActions::roomUUID = responseJSON["roomUUID"].get<std::string>();
	UserActions::role = responseJSON["role"].get<std::string>();

	UserActions::socket.connect("127.0.0.1", 52001);

	UserActions::socket.send(UserActions::roomUUID);

	curl_slist_free_all(headers);
}

TEST_F(UserActions, UpdateName)
{
	std::string jsonData = std::format(R"({{"userUUID": "{}", "newUserName": "{}"}})", UserActions::userUUID, UserActions::newUserName);
	uint8_t eventId;
	std::string event;

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:52000/users"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	ASSERT_EQ(UserActions::response, "User name has been changed");

	UserActions::socket.receive(&eventId);

	ASSERT_EQ(eventId, 3);

	{
		int size;
		std::string data;

		UserActions::socket.receive(&size);

		data.resize(size);

		UserActions::socket.receive(data);

		std::istringstream is(data);

		std::string temp;

		is >> temp;

		ASSERT_EQ(temp, UserActions::userName);

		is >> temp;

		ASSERT_EQ(temp, UserActions::newUserName);
	}

	curl_slist_free_all(headers);
}

TEST_F(UserActions, GetRoomInformation)
{
	std::string url = std::format("http://127.0.0.1:52000/users?user_uuid={}", UserActions::userUUID);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(UserActions::response);

	ASSERT_EQ(responseJSON["name"].get<std::string>(), UserActions::newUserName);
	ASSERT_EQ(responseJSON["role"].get<std::string>(), "owner");
}

TEST_F(UserActions, UploadContent)
{
	for (std::string_view contentName : UserActions::getContentFiles())
	{
		std::string url = std::format("http://127.0.0.1:52000/upload/{}/{}/{}", UserActions::roomUUID, UserActions::userUUID, contentName);
		uint8_t eventId;
		std::string event(36, '\0');
		FILE* file = fopen(contentName.data(), "rb");

		ASSERT_NE(file, nullptr);

		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);
		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L), CURLE_OK);
		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_READDATA, file), CURLE_OK);
		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, std::filesystem::file_size(contentName)), CURLE_OK);

		ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

		ASSERT_RESPONSE_CODE(201);

		ASSERT_EQ(UserActions::response, "File uploaded");

		UserActions::socket.receive(&eventId);
		UserActions::socket.receive(event);

		ASSERT_EQ(eventId, 0);
		ASSERT_EQ(event, UserActions::userUUID);

		fclose(file);
		UserActions::response.clear();
	}
}

TEST_F(UserActions, UserUploadedContent)
{
	std::string url = std::format("http://127.0.0.1:52000/users?user_uuid={}", UserActions::userUUID);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(UserActions::response);
	const auto& content = responseJSON["uploadedContent"];

	ASSERT_EQ(content.size(), UserActions::getContentFiles().size());

	auto contentFiles = UserActions::getContentFiles();

	for (size_t i = 0; i < contentFiles.size(); i++)
	{
		ASSERT_EQ(content[i].get<std::string>(), contentFiles[i]);
	}
}

TEST_F(UserActions, AvailableContent)
{
	std::string url = std::format("http://127.0.0.1:52000/content/{}", UserActions::roomUUID);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(UserActions::response);
	const auto& content = responseJSON["uploadedContent"];

	ASSERT_EQ(content.size(), UserActions::getContentFiles().size());

	auto contentFiles = UserActions::getContentFiles();

	for (size_t i = 0; i < contentFiles.size(); i++)
	{
		ASSERT_EQ(content[i].get<nlohmann::json>()["contentName"].get<std::string>(), contentFiles[i]);
		ASSERT_EQ(content[i].get<nlohmann::json>()["userName"].get<std::string>(), UserActions::newUserName);
	}
}

TEST_F(UserActions, DownloadContent)
{
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &UserActions::writeFileCallback), CURLE_OK);

	for (std::string_view contentName : UserActions::getContentFiles())
	{
		std::string url = std::format("http://127.0.0.1:52000/download/{}/{}", UserActions::roomUUID, contentName);
		std::string downloadedFileName = std::format("downloaded_{}", contentName);
		std::ofstream out(downloadedFileName, std::ios::binary);

		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);
		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out), CURLE_OK);

		ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

		out.flush();

		ASSERT_TRUE(UserActions::compareFiles(contentName, downloadedFileName));

		UserActions::response.clear();
	}
}

TEST_F(UserActions, DeleteRoom)
{
	std::string url = std::format("http://127.0.0.1:52000/rooms?room_uuid={}", UserActions::roomUUID);
	uint8_t eventId;

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE"), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	ASSERT_RESPONSE_CODE(200);

	UserActions::socket.receive(&eventId);

	ASSERT_EQ(eventId, 4);

	ASSERT_EQ(UserActions::response, "Room deleted");
}
