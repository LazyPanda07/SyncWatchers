#include "EventsManager.h"

#include "Server.h"

static std::unique_ptr<events::EventsManager> instance;

namespace events
{
	void EventsManager::addSocket(SOCKET socket)
	{
		std::string uuid(36, '\0');

		recv(socket, uuid.data(), static_cast<int>(uuid.size()), 0);

		std::lock_guard<std::mutex> lock(dataMutex);

		if (auto it = data.find(uuid); it != data.end())
		{
			it->second.subscribers.push_back(socket);
		}
		else
		{
			data.try_emplace(uuid, std::vector<SOCKET>({ socket }));
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

	void EventsManager::notify(const IEvent& event, const std::string& roomUUID)
	{
		EventId eventId = event.getEventId();
		std::string eventData(event.getEventData());

		std::thread
		(
			[this, roomUUID = roomUUID, eventId, eventData = std::move(eventData)]()
			{
				std::vector<SOCKET> sockets;
				std::vector<SOCKET> disconnected;
				std::mutex* sendMutex = nullptr;

				{
					std::lock_guard<std::mutex> lock(dataMutex);

					if (auto it = data.find(roomUUID); it != data.end())
					{
						sockets = it->second.subscribers;
						sendMutex = &it->second.sendMutex;
					}
				}

				if (!sendMutex)
				{
					return;
				}

				static_assert(sizeof(eventId) == 1);

				{
					std::lock_guard<std::mutex> lock(*sendMutex);

					for (SOCKET socket : sockets)
					{
						if (send(socket, reinterpret_cast<const char*>(&eventId), sizeof(eventId), 0) == SOCKET_ERROR)
						{
							disconnected.push_back(socket);

							continue;
						}

						if (eventData.size())
						{
							if (send(socket, eventData.data(), static_cast<int>(eventData.size()), 0) == SOCKET_ERROR)
							{
								disconnected.push_back(socket);
							}
						}
					}
				}

				{
					std::lock_guard<std::mutex> lock(dataMutex);

					if (auto it = data.find(roomUUID); it != data.end())
					{
						for (SOCKET socket : disconnected)
						{
							it->second.subscribers.erase(std::find(it->second.subscribers.begin(), it->second.subscribers.end(), socket));
						}
					}
				}
			}
		).detach();
	}

	size_t EventsManager::getListeners() const
	{
		std::lock_guard<std::mutex> lock(dataMutex);
		size_t result = 0;

		for (const auto& [_, room] : data)
		{
			result += room.subscribers.size();
		}

		return result;
	}
}

events::IEventsManager* getEventsManager()
{
	return &events::EventsManager::getInstance();
}
