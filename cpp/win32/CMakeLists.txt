cmake_minimum_required(VERSION 3.20)

project(HelloWIN32)

include(FetchContent)

FetchContent_Declare(
  wil
  URL  https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.211019.2
)
# Microsoft.Web.WebView2
FetchContent_Declare(
  webview2
  URL  https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.1072.54
)

FetchContent_Declare(
  miniz
  URL  https://github.com/richgel999/miniz/releases/download/3.1.0/miniz-3.1.0.zip
)
FetchContent_MakeAvailable(wil webview2 miniz)


add_executable(${PROJECT_NAME} WIN32 main.cpp ${miniz_SOURCE_DIR}/miniz.c)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(PLATFORM "x86")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE "${webview2_SOURCE_DIR}/build/native/${PLATFORM}/WebView2LoaderStatic.lib")
target_link_libraries(${PROJECT_NAME} PRIVATE shlwapi)
target_include_directories(${PROJECT_NAME} PRIVATE "${wil_SOURCE_DIR}/include" "${webview2_SOURCE_DIR}/build/native/include" "${miniz_SOURCE_DIR}")

# Function to embed binary files as C++ arrays
function(embed_binary_file target)
    set(one_value_args NAMESPACE HEADER_NAME)
    set(multi_value_args SOURCES)
    cmake_parse_arguments(ARG "" "${one_value_args}" "${multi_value_args}" ${ARGN})

    message("erik ${ARG_SOURCES}")

    file(READ ${ARG_SOURCES} HEX_DATA HEX)
    string(LENGTH "${HEX_DATA}" HEX_LENGTH)
    math(EXPR BYTE_COUNT "${HEX_LENGTH} / 2")
    
    # Convert hex string to comma-separated values (much faster)
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," ARRAY_DATA ${HEX_DATA})
    string(REGEX REPLACE ",$" "" ARRAY_DATA ${ARRAY_DATA})

    foreach(src IN LISTS ARG_SOURCES)
        cmake_path(GET src STEM stem)
    endforeach()

    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Data/Data.cpp
"#include <array>
#include <cstdint>
#include \"Data.h\"

namespace ${ARG_NAMESPACE} {
    const std::array<uint8_t, ${BYTE_COUNT}> ${stem} = {${ARRAY_DATA}};
}
")


    
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Data/Data.h
"#pragma once
#include <array>
#include <cstdint>

namespace ${ARG_NAMESPACE} {
    extern const std::array<uint8_t,${BYTE_COUNT}> ${stem};
}
")

add_library(${target} STATIC ${CMAKE_CURRENT_BINARY_DIR}/Data/Data.cpp)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Data/)

endfunction()

  embed_binary_file(${PROJECT_NAME}Data 
      SOURCES ui/dist.zip
      HEADER_NAME "embedded_data.h"
      NAMESPACE Can)
  # target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Data)