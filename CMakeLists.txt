### Warning: Cool's CMakeLists.txt currently assumes that the project using Cool created its executable target with the same name as its project:
### ```cmake
### project(CoolLab)                # Choose a name
### add_executable(${PROJECT_NAME}) # Use that name
### ```

cmake_minimum_required(VERSION 3.20)

add_library(Cool)
add_library(Cool::Core ALIAS Cool)
target_compile_features(Cool PUBLIC cxx_std_20)
if (MSVC)
    target_compile_options(Cool PRIVATE /WX /W3)
else()
    target_compile_options(Cool PRIVATE -Werror -Wall -Wextra -Wpedantic -pedantic-errors)
endif()

# Set COOL_PATH as the path to Cool, relative to the main project's CMakeLists.txt. It will look like "third-party/Cool"
cmake_path(RELATIVE_PATH CMAKE_CURRENT_SOURCE_DIR BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE COOL_PATH)

target_compile_definitions(Cool PUBLIC
    $<$<CONFIG:Debug>:DEBUG> # Define DEBUG in debug mode
    COOL_PATH=\"${COOL_PATH}\" # This is useful to set the path to Cool's res directory
    $<$<CONFIG:Debug>:COOL_DEBUG_ONLY__CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"> # This is useful to set the current working directory in debug mode.
)

# ----------------------
# ---Add Cool sources---
# ----------------------

file(GLOB_RECURSE COOL_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(Cool PRIVATE ${COOL_SOURCES})

# -----------------------------
# ---Set include directories---
# -----------------------------

target_include_directories(Cool PUBLIC src lib)
target_precompile_headers(Cool PUBLIC # More info on precompiled headers: https://julesfouchy.github.io/Learn--Cpp-And-Dev-Practices/docs/lessons/precompiled-header
    <vector>
    <string>
    <memory>
    <functional>
    <variant>
    <optional>
    <algorithm>
    <unordered_map>
    <deque>
    <thread>
    <utility>
    <chrono>
    <list>
    <imgui/imgui.h>
    <imgui/misc/cpp/imgui_stdlib.h>
    <glm/glm.hpp>
    <glm/gtc/type_ptr.hpp>
    <cereal/access.hpp>
    <cereal/types/vector.hpp>
    <cereal/types/string.hpp>
    <cereal/types/memory.hpp>
    <cereal/types/variant.hpp>
    <boxer/boxer.h>
    <nfd.hpp>
    <scope_guard/scope_guard.hpp>
    <Cool/Log/Debug.h>
    <Cool/Serialization/internal/GlmSerialization.h> # must be included after <glm/glm.hpp> because it depends on it
    <Cool/ostream/glm.h>                             # must be included after <glm/glm.hpp> because it depends on it
    <Cool/ostream/vector.h>                          # must be included after <vector> because it depends on it
)
if (COOL_USE_VULKAN)
    target_precompile_headers(Cool PUBLIC
        <vulkan/vulkan.hpp>
        <vku/vku.hpp>
    )
elseif (COOL_USE_OPENGL)
    target_precompile_headers(Cool PUBLIC
        <glad/glad.h>
        <Cool/Gpu/OpenGL/GLDebug.h>
    )
endif()

# -------------------------
# ---Add Vulkan / OpenGL---
# -------------------------

if (COOL_USE_VULKAN)
    target_compile_definitions(Cool PUBLIC 
        COOL_VULKAN
        COOL_VULKAN_VERSION=${COOL_USE_VULKAN})
    find_package(Vulkan REQUIRED)
    target_link_libraries(Cool PUBLIC Vulkan::Vulkan)
    add_subdirectory(lib/Vookoo/include)
    target_link_libraries(Cool PUBLIC Vookoo::Vookoo)
    add_subdirectory(lib/easy-shaderc)
    target_link_libraries(Cool PUBLIC easy_shaderc::shaderc)
elseif (COOL_USE_OPENGL)
    target_compile_definitions(Cool PUBLIC
        COOL_OPENGL
        COOL_OPENGL_VERSION=${COOL_USE_OPENGL})
    add_subdirectory(lib/glpp)
    target_compile_definitions(glpp PRIVATE $<$<CONFIG:Debug>:GLPP_SHOULD_CHECK_ERRORS>)
    target_link_libraries(Cool PUBLIC glpp::glpp)
endif()

# -------------------
# ---Add libraries---
# -------------------

# algorithms
add_subdirectory(lib/algorithms)
target_link_libraries(Cool PUBLIC algorithms::algorithms)
# GLFW
add_subdirectory(lib/glfw)
target_link_libraries(Cool PUBLIC glfw)
# glm
add_subdirectory(lib/glm)
target_link_libraries(Cool PUBLIC glm::glm)
# cereal
target_include_directories(Cool PUBLIC lib/cereal/include)
# spdlog
add_subdirectory(lib/spdlog)
target_link_libraries(Cool PUBLIC spdlog::spdlog_header_only)
# smart
add_subdirectory(lib/smart)
target_link_libraries(Cool PUBLIC smart::smart)
# Boxer
add_subdirectory(lib/Boxer)
target_link_libraries(Cool PUBLIC Boxer)
# Native File Dialog
add_subdirectory(lib/nativefiledialog-extended EXCLUDE_FROM_ALL)
target_link_libraries(Cool PUBLIC nfd)
# DearWidgets
add_subdirectory(lib/DearWidgets)
target_link_libraries(Cool PUBLIC DearWidgets::DearWidgets)
# img
add_subdirectory(lib/img)
target_link_libraries(Cool PUBLIC img::img)
# exe_path
add_subdirectory(lib/exe_path)
target_link_libraries(Cool PUBLIC exe_path::exe_path)
# serv
add_subdirectory(lib/serv)
target_link_libraries(Cool PUBLIC serv::serv)
# stringify
add_subdirectory(lib/stringify)
target_link_libraries(Cool PUBLIC stringify::stringify)
# type_from_string
add_subdirectory(lib/type_from_string)
target_link_libraries(Cool PUBLIC type_from_string::type_from_string)
# op
add_subdirectory(lib/op)
target_link_libraries(Cool PUBLIC op::op)
# reg
add_subdirectory(lib/reg)
target_link_libraries(Cool PUBLIC reg::reg)
# wafl
add_subdirectory(lib/wafl)
target_link_libraries(Cool PUBLIC wafl::wafl)
# open_link
add_subdirectory(lib/open_link)
target_link_libraries(Cool PUBLIC open_link::open_link)
# doctest
add_subdirectory(lib/doctest)
target_link_libraries(Cool PUBLIC doctest::doctest)
target_compile_definitions(Cool PUBLIC $<$<NOT:$<CONFIG:Debug>>:DOCTEST_CONFIG_DISABLE>) # Strip tests from release builds
# Dear ImGui
add_subdirectory(lib/imgui)
    # Build ImGui with glfw
    target_link_libraries(ImGui PRIVATE glfw)
    target_sources(ImGui PRIVATE lib/imgui/backends/imgui_impl_glfw.cpp)
    # Build ImGui with Vulkan / OpenGL
    if (COOL_USE_VULKAN)
        target_sources(ImGui PRIVATE lib/imgui/backends/imgui_impl_vulkan.cpp)
        target_link_libraries(ImGui PRIVATE Vulkan::Vulkan)
    elseif (COOL_USE_OPENGL)
        target_sources(ImGui PRIVATE lib/imgui/backends/imgui_impl_opengl3.cpp)
    endif()
target_link_libraries(Cool PUBLIC ImGui::ImGui)

# ----------------
# ---Copy files---
# ----------------

include("CMakeUtils/files_and_folders.cmake")
Cool__create_file_if_it_doesnt_exist(${CMAKE_SOURCE_DIR}/imgui.ini) # This prevents the following copy command from failing
Cool__target_copy_file(${PROJECT_NAME} Cool
    imgui.ini)
Cool__target_copy_folder(${PROJECT_NAME} Cool
    ${COOL_PATH}/res)
    
function(cool_setup TARGET_NAME)
    Cool__target_copy_file(${TARGET_NAME} Cool imgui.ini)
    Cool__target_copy_folder(${TARGET_NAME} Cool ${COOL_PATH}/res)
endfunction()

# ---------------------
# ---Setup the tests---
# ---------------------

add_executable(Tests-Cool tests/tests.cpp ${COOL_SOURCES})
target_compile_features(Tests-Cool PRIVATE cxx_std_20)
if(MSVC)
    target_compile_options(Tests-Cool PRIVATE /WX /W3)
else()
    target_compile_options(Tests-Cool PRIVATE -Werror -Wall -Wextra -Wpedantic -pedantic-errors)
endif()
target_link_libraries(Tests-Cool PRIVATE Cool::Core)