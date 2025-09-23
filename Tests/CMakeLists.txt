cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(FETCHCONTENT_QUIET OFF CACHE BOOL "")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)

set(CONTENT_TEST_FILE ${CMAKE_INSTALL_PREFIX}/content.mp4)

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

find_package(reproc++ QUIET)

if (NOT ${reproc++_FOUND})
	set(REPROC_TAG v14.2.5)

	FetchContent_Declare(
		reproc++
		GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
		GIT_TAG ${REPROC_TAG}
	)

	set(REPROC++ ON CACHE BOOL "" FORCE)

	FetchContent_MakeAvailable(reproc++)
endif()

add_custom_target(
	build_sync_watchers ALL
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/_deps/SyncWatchers
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -S ${CMAKE_SOURCE_DIR}/../ -B ${CMAKE_BINARY_DIR}/_deps/SyncWatchers
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/_deps/SyncWatchers -j --config Release
	COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR}/_deps/SyncWatchers --config Release
)

if(UNIX)
	set(GENERATE_CMD dd if=/dev/zero of=${CONTENT_TEST_FILE} bs=1M count=10)
else()
	set(GENERATE_CMD powershell -Command "[IO.File]::WriteAllBytes('${CONTENT_TEST_FILE}', (New-Object Byte[](10485760)))")
endif()

add_custom_command(
    OUTPUT ${CONTENT_TEST_FILE}
    COMMAND ${GENERATE_CMD}
    COMMENT "Generating 10 MiB content.mp4 file"
)

add_custom_target(generate_test_content ALL DEPENDS ${CONTENT_TEST_FILE})

add_executable(
	${PROJECT_NAME}
	main.cpp
	cases/basic_functionality.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	GTest::gtest
	GTest::gtest_main
	CURL::libcurl
	nlohmann_json::nlohmann_json
	reproc++
)

add_dependencies(
	${PROJECT_NAME}
	build_sync_watchers
)

enable_testing()

add_test(NAME BasicFunctionality COMMAND ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION .)
