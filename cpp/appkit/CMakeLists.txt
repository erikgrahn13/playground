cmake_minimum_required(VERSION 3.22)

project(hello LANGUAGES Swift)

set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum OS X deployment version" FORCE)

add_executable(hello main.swift)

# Set Swift compilation flags
set_target_properties(hello PROPERTIES
    Swift_LANGUAGE_VERSION 6
    MACOSX_BUNDLE TRUE
)

# # Add the required Swift compiler flag for @main
target_compile_options(hello PRIVATE 
    "$<$<COMPILE_LANGUAGE:Swift>:-parse-as-library>"
)