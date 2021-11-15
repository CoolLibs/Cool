# Master CMake configuration to include shaderc and its dependency projects.
# In this way we can track all projects under the same directory, instead
# of using the directory hierarchy required by shaderc, which Git submodule
# does not like.

cmake_minimum_required(VERSION 2.8.12)

project(shaderc-and-deps)

set(SHADERC_SKIP_INSTALL true)
set(SHADERC_SKIP_TESTS true)
set(SHADERC_SKIP_EXAMPLES true)
set(SHADERC_ENABLE_SHARED_CRT ON)
set(SPIRV_HEADERS_ENABLE_EXAMPLES OFF)
set(SPIRV_HEADERS_ENABLE_INSTALL OFF)
add_subdirectory(SPIRV-Headers)
add_subdirectory(SPIRV-Tools)
add_subdirectory(glslang)
add_subdirectory(shaderc)
