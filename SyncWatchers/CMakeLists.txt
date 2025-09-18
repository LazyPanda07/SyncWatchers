cmake_minimum_required(VERSION 3.27.0)

project(SyncWatchers)

add_executable(
	${PROJECT_NAME}
	src/Main.cpp
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/config.json DESTINATION .)
