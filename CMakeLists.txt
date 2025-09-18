cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(FETCHCONTENT_QUIET OFF CACHE BOOL "")

project(SyncWatchersProject)

include(FetchWebFramework/FetchWebFramework.cmake)

find_package(WebFramework REQUIRED)

add_subdirectory(SyncWatchers)
add_subdirectory(Executors)
