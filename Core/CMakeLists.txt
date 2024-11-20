cmake_minimum_required(VERSION 3.27.0)

project(Core)

add_library(
	${PROJECT_NAME} SHARED
	source/executors/Rooms.cpp
	source/executors/Users.cpp
	source/models/RoomModel.cpp
	source/models/UserModel.cpp
	source/models/UserToRoomModel.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	source/
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION executors/)
install(FILES core.json DESTINATION executors/)
