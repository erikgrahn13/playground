cmake_minimum_required(VERSION 3.22)

project(coreaudio)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    main.cpp
    AudioDevice.cpp
    utilities/CARingBuffer.cpp # temporary (should write my own)
    utilities/CAStreamBasicDescription.cpp
    )

# Link CoreAudio and CoreFoundation frameworks
target_link_libraries(${PROJECT_NAME} PRIVATE 
    "-framework CoreAudio"
    "-framework CoreFoundation"
    "-framework AudioToolbox"
)