cmake_minimum_required(VERSION 3.16)
project(NativeWebView LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific source files
set(SOURCES
    src/main.cpp
    src/webview_wrapper.cpp
    src/resource_manager.cpp
)

# Add embedded resources for release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(EMBEDDED_RESOURCES_FILE "${CMAKE_BINARY_DIR}/embedded_resources.cpp")
    
    # Custom command to embed web resources
    add_custom_command(
        OUTPUT ${EMBEDDED_RESOURCES_FILE}
        COMMAND ${CMAKE_SOURCE_DIR}/scripts/embed_resources.sh ${CMAKE_SOURCE_DIR}/ui/dist ${EMBEDDED_RESOURCES_FILE}
        DEPENDS ${CMAKE_SOURCE_DIR}/ui/dist
        COMMENT "Embedding web resources..."
    )
    
    list(APPEND SOURCES ${EMBEDDED_RESOURCES_FILE})
endif()

if(APPLE)
    list(APPEND SOURCES src/platform/macos/webview_macos.mm)
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(WEBKIT_FRAMEWORK WebKit)
    set(PLATFORM_LIBS ${COCOA_FRAMEWORK} ${WEBKIT_FRAMEWORK})
    # Enable ARC for Objective-C++ files
    set_source_files_properties(src/platform/macos/webview_macos.mm PROPERTIES COMPILE_FLAGS "-fobjc-arc")
elseif(WIN32)
    list(APPEND SOURCES src/platform/windows/webview_windows.cpp)
    # WebView2 dependencies
    find_package(PkgConfig)
    set(PLATFORM_LIBS webview2loader ole32 oleaut32 user32 gdi32)
elseif(UNIX)
    list(APPEND SOURCES src/platform/linux/webview_linux.cpp)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.0)
    set(PLATFORM_LIBS ${WEBKIT_LIBRARIES})
    include_directories(${WEBKIT_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${PLATFORM_LIBS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Compile definitions
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE=1)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE RELEASE_MODE=1)
endif()
