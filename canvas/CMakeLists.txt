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

FetchContent_Declare(
    visage
    GIT_REPOSITORY https://github.com/VitalAudio/visage.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/visage
)

FetchContent_MakeAvailable(visage)


add_executable(main main.cpp)
target_link_libraries(main PRIVATE qjs-libc visage)
target_include_directories(main PRIVATE ${quickjs_SOURCE_DIR})
target_compile_features(main PUBLIC cxx_std_23)
