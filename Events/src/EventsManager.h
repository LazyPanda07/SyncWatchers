#pragma once

#include "Server.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "IEventsManager.h"

#ifdef __LINUX__
#define EVENTS_MANAGER_API extern "C" __attribute__((visibility("default")))
#else
#define EVENTS_MANAGER_API extern "C" __declspec(dllexport)
#endif

namespace events
{
	class EventsManager : public IEventsManager
	{
	private:
		std::unordered_map<std::string, std::vector<SOCKET>> connections;
		std::mutex connectionsMutex;
		std::unique_ptr<Server> server;

	private:
		EventsManager() = default;

		~EventsManager() = default;

	private:
		void addSocket(SOCKET socket);

	public:
		static EventsManager& getInstance();

		void startServer(std::string_view ip, int64_t port) override;

		friend struct std::default_delete<EventsManager>;
		friend class Server;
	};
}

EVENTS_MANAGER_API events::IEventsManager* getEventsManager();
