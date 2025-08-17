cmake_minimum_required(VERSION 3.24)

project(sndrv-ctrl-erik VERSION 0.1.0 LANGUAGES C CXX)

add_library(${PROJECT_NAME} SHARED src/main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_executable(dummy src/dummy.c)