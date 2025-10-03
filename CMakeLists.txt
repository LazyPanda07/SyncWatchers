cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(FETCHCONTENT_QUIET OFF CACHE BOOL "")
option(WITH_CLIENTS "Build Windows and Android client apps" OFF)
option(WITH_WINDOWS_CLIENT "Build Windows client app" OFF)
option(WITH_ANDROID_CLIENT "Build Android client app" OFF)

project(SyncWatchersProject VERSION 0.1.0)

include(FetchWebFramework/FetchWebFramework.cmake)

find_package(WebFramework REQUIRED)

add_subdirectory(Utils)
add_subdirectory(Events)
add_subdirectory(SyncWatchers)
add_subdirectory(Executors)

if (${WITH_CLIENTS} OR ${WITH_WINDOWS_CLIENT} OR ${WITH_ANDROID_CLIENT})
	add_subdirectory(SyncWatchersClient)
endif()
