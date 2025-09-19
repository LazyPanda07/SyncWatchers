cmake_minimum_required(VERSION 3.27.0)

project(Tests)

include(CTest)
enable_testing()

find_package(GTest QUIET)

if (NOT ${GTest_FOUND})
	set(GTEST_TAG v1.17.0)

	include(FetchContent)

	FetchContent_Declare(
		GoogleTest
		GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${GTEST_TAG}
	)

	FetchContent_MakeAvailable(GoogleTest)
endif()

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	gtest
	gtest_main
)

add_test(NAME Rooms COMMAND ${PROJECT_NAME})
add_test(NAME Users COMMAND ${PROJECT_NAME})
