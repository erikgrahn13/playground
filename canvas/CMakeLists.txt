cmake_minimum_required(VERSION 3.22)

project(canvas)

include(FetchContent)
FetchContent_Declare(
    quickjs
    URL https://github.com/quickjs-ng/quickjs/archive/refs/tags/v0.12.1.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/quickjs
)

FetchContent_MakeAvailable(quickjs)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE qjs-libc)
target_include_directories(main PRIVATE ${quickjs_SOURCE_DIR})