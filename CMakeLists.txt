cmake_minimum_required(VERSION 3.27.0)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "")

project(SyncWatchersProject)

include(cmake/FetchWebFramework.cmake)

add_subdirectory(SyncWatchers)
