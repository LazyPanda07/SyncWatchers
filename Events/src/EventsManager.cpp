#include "EventsManager.h"

#include "Server.h"

static std::unique_ptr<events::EventsManager> instance;

namespace events
{
	void EventsManager::addSocket(SOCKET socket)
	{
		std::string uuid(36, '\0');

		recv(socket, uuid.data(), static_cast<int>(uuid.size()), NULL);

		std::lock_guard<std::mutex> lock(connectionsMutex);

		if (auto it = connections.find(uuid); it != connections.end())
		{
			it->second.push_back(socket);
		}
		else
		{
			connections.try_emplace(uuid, std::vector<SOCKET>({ socket }));
		}
	}

	EventsManager& EventsManager::getInstance()
	{
		if (!instance)
		{
			instance = std::unique_ptr<EventsManager>(new EventsManager());
		}

		return *instance;
	}

	void EventsManager::startServer(std::string_view ip, int64_t port)
	{
		server = std::make_unique<Server>(ip, port + 1);

		server->start();
	}

	size_t EventsManager::getListeners() const
	{
		std::lock_guard<std::mutex> lock(connectionsMutex);
		size_t result = 0;
		
		for (const auto& [_, value] : connections)
		{
			result += value.size();
		}

		return result;
	}
}

events::IEventsManager* getEventsManager()
{
	return &events::EventsManager::getInstance();
}
