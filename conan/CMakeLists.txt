cmake_minimum_required(VERSION 3.20)

project(hello-conan)

find_package(GTest REQUIRED)

enable_testing()

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} gtest::gtest)

add_test(NAME test COMMAND ${PROJECT_NAME})
