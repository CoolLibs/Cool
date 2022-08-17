cmake_minimum_required(VERSION 3.20)
project(Tests-Cool)
add_subdirectory(.. ${CMAKE_CURRENT_SOURCE_DIR}/build/Cool)
cool_setup(${PROJECT_NAME})