cmake_minimum_required(VERSION 3.20)
project(Tests-Cool)
set(COOL_USE_OPENGL $<IF:$<PLATFORM_ID:Darwin>,410,430>)
add_subdirectory(.. ${CMAKE_CURRENT_SOURCE_DIR}/build/Cool)