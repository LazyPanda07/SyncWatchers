cmake_minimum_required(VERSION 3.27.0)

project(VideoExecutor)

add_library(
	${PROJECT_NAME} SHARED
	src/StopExecutor.cpp
	src/Events/StopEvent.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	Utils
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/Events/src
	${CMAKE_SOURCE_DIR}/Utils/src
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES video_config.json DESTINATION .)
install(FILES video_web.json DESTINATION .)
