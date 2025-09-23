include(FetchContent)

set(WEB_FRAMEWORK_VERSION 3.1.2)
set(WEB_FRAMEWORK_URL https://github.com/LazyPanda07/WebFramework/releases/download/v${WEB_FRAMEWORK_VERSION})

if (UNIX)
    set(WEB_FRAMEWORK_URL ${WEB_FRAMEWORK_URL}/linux.zip)
else()
    set(WEB_FRAMEWORK_URL ${WEB_FRAMEWORK_URL}/windows.zip)
endif()

FetchContent_Declare(
    WebFramework
    URL ${WEB_FRAMEWORK_URL}
)

FetchContent_MakeAvailable(WebFramework)

if (UNIX)
    set(WEB_FRAMEWORK_SDK ${webframework_SOURCE_DIR})
else()
    set(WEB_FRAMEWORK_SDK ${webframework_SOURCE_DIR}/Release_Windows)
endif()

list(APPEND CMAKE_PREFIX_PATH ${WEB_FRAMEWORK_SDK})
