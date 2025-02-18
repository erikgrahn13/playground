cmake_minimum_required(VERSION 3.25)

# set(CMAKE_SYSTEM_NAME Windows)
# set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
# set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
# set(CMAKE_BUILD_TYPE Debug)  # Enables debug mode
# set(CMAKE_CXX_FLAGS "-g -O0")  # Add debug symbols and disable optimizations


project(asio-example)



include(FetchContent)

FetchContent_Declare(
    asiosdk
    URL https://www.steinberg.net/asiosdk
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/asiosdk
)

FetchContent_MakeAvailable(asiosdk)

add_library(asio STATIC
    ${asiosdk_SOURCE_DIR}/common/asio.cpp
    ${asiosdk_SOURCE_DIR}/host/asiodrivers.cpp
    ${asiosdk_SOURCE_DIR}/host/pc/asiolist.cpp
)
target_include_directories(asio PUBLIC 
    ${asiosdk_SOURCE_DIR}/common
    ${asiosdk_SOURCE_DIR}/host
    ${asiosdk_SOURCE_DIR}/host/pc    
)


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE asio)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
