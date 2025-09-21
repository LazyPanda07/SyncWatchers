cmake_minimum_required(VERSION 3.27.0)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)

project(Tests)

include(CTest)
include(FetchContent)

find_package(GTest QUIET)

if (NOT ${GTest_FOUND})
	set(GTEST_TAG v1.17.0)

	FetchContent_Declare(
		GoogleTest
		GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${GTEST_TAG}
	)

	FetchContent_MakeAvailable(GoogleTest)
endif()

find_package(CURL QUIET)

if (NOT ${CURL_FOUND})
	set(CURL_VERSION 8_16_0)
	set(CURL_TAG curl-${CURL_VERSION})

	FetchContent_Declare(
		CURL
		GIT_REPOSITORY https://github.com/curl/curl.git
		GIT_TAG ${CURL_TAG}
	)

	set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
	set(HTTP_ONLY ON CACHE BOOL "" FORCE)
	set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)

	FetchContent_MakeAvailable(CURL)
endif()

find_package(nlohmann_json QUIET)

if (NOT ${nlohmann_json_FOUND})
	set(NLOHMANN_JSON_TAG v3.12.0)

	FetchContent_Declare(
		nlohmann_json
		GIT_REPOSITORY https://github.com/nlohmann/json.git
		GIT_TAG ${JSON_TAG}
	)

	FetchContent_MakeAvailable(nlohmann_json)
endif()

add_custom_target(
	build_sync_watchers ALL
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/SyncWatchers
	COMMAND ${CMAKE_COMMAND} -E echo "Directory: ${CMAKE_BINARY_DIR}, Source directory: ${CMAKE_SOURCE_DIR}/../"
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/../
	# COMMAND ${CMAKE_COMMAND} --build . -j --config Release
	# COMMAND ${CMAKE_COMMAND} --install . --config Release
)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	gtest
	gtest_main
	libcurl
	nlohmann_json::nlohmann_json
)

add_dependencies(
	${PROJECT_NAME}
	build_sync_watchers
)

enable_testing()

add_test(NAME Rooms COMMAND ${PROJECT_NAME})
add_test(NAME Users COMMAND ${PROJECT_NAME})
