#include <fstream>
#include <filesystem>
#include <format>

#include <curl/curl.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "utils/Socket.h"

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

	curl_slist* headers = nullptr;

	headers = curl_slist_append(headers, "Content-Type: application/json");

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(UserActions::response);

	UserActions::inviteLink = responseJSON["inviteLink"].get<std::string>();

	curl_slist_free_all(headers);
}

TEST_F(UserActions, JoinRoom)
{
	constexpr std::string_view jsonData = R"({"role": "owner"})";

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, inviteLink.data()), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	auto responseJSON = nlohmann::json::parse(UserActions::response);

	UserActions::userName = responseJSON["userName"].get<std::string>();
	UserActions::userUUID = responseJSON["userUUID"].get<std::string>();
	UserActions::roomUUID = responseJSON["roomUUID"].get<std::string>();
	UserActions::role = responseJSON["role"].get<std::string>();

	UserActions::socket.connect("127.0.0.1", 52001);

	UserActions::socket.send(UserActions::roomUUID);
}

TEST_F(UserActions, UpdateName)
{
	std::string jsonData = std::format(R"({{"userUUId": "{}", "newUserName": "{}"}})", UserActions::userUUID, UserActions::newUserName);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:52000/users"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	curl_slist* headers = nullptr;

	curl_slist_append(headers, "Content-Type: application/json");

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

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
		FILE* file = fopen(contentName.data(), "rb");

		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);
		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L), CURLE_OK);
		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_READDATA, file), CURLE_OK);
		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, std::filesystem::file_size(contentName)), CURLE_OK);

		ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

		ASSERT_EQ(UserActions::response, "File uploaded");

		ASSERT_TRUE(UserActions::compareFiles(contentName, std::filesystem::current_path() / "assets" / UserActions::roomUUID / contentName));

		fclose(file);
		UserActions::response.clear();
	}
}

TEST_F(UserActions, AvailableContent)
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

TEST_F(UserActions, DownloadContent)
{
	for (std::string_view contentName : UserActions::getContentFiles())
	{
		std::string url = std::format("http://127.0.0.1:52000/download/{}/{}", UserActions::roomUUID, contentName);

		ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, url.data()), CURLE_OK);

		ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

		std::ofstream(contentName.data(), std::ios::binary).write(UserActions::response.data(), UserActions::response.size());

		ASSERT_TRUE(UserActions::compareFiles(contentName, std::filesystem::current_path() / "assets" / UserActions::roomUUID / contentName));

		UserActions::response.clear();
	}
}

TEST_F(UserActions, DeleteRoom)
{
	std::string jsonData = std::format(R"({{"room_uuid": "{}", }})", UserActions::roomUUID);

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:52000/rooms"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE"), CURLE_OK);
	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.data()), CURLE_OK);

	curl_slist* headers = nullptr;

	headers = curl_slist_append(headers, "Content-Type: application/json");

	ASSERT_EQ(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers), CURLE_OK);

	ASSERT_EQ(curl_easy_perform(curl), CURLE_OK);

	curl_slist_free_all(headers);

	ASSERT_EQ(UserActions::response, "Room deleted");
}
