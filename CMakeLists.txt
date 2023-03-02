# On top of the usual `target_link_libraries`, you must also call `cool_setup` with your target name. E.g.:
# ```
# add_subdirectory(Cool)
# target_link_libraries(${PROJECT_NAME} PRIVATE Cool::Core)
# cool_setup(${PROJECT_NAME})
# ```

cmake_minimum_required(VERSION 3.20)

add_library(Cool)
add_library(Cool::Core ALIAS Cool)
add_library(Cool-PublicProperties INTERFACE) # This is needed by both Cool and its tests
add_library(Cool-PrivateProperties INTERFACE) # This is needed by both Cool and its tests
target_link_libraries(Cool PUBLIC Cool-PublicProperties)
target_link_libraries(Cool PRIVATE Cool-PrivateProperties)

# Choose C++ version
target_compile_features(Cool-PublicProperties INTERFACE cxx_std_20)

# Set warning level
if(MSVC)
    target_compile_options(Cool-PrivateProperties INTERFACE /W3)
else()
    target_compile_options(Cool-PrivateProperties INTERFACE -Wall -Wextra -Wpedantic -pedantic-errors)
endif()

# Maybe enable warnings as errors
set(WARNINGS_AS_ERRORS_FOR_COOL OFF CACHE BOOL "ON iff you want to treat warnings as errors") # Might be overriden in the CMake cache

if(WARNINGS_AS_ERRORS_FOR_COOL)
    if(MSVC)
        target_compile_options(Cool-PrivateProperties INTERFACE /WX)
    else()
        target_compile_options(Cool-PrivateProperties INTERFACE -Werror)
    endif()
endif()

# Set COOL_PATH as the path to Cool, relative to the main project's CMakeLists.txt. It will look like "third-party/Cool"
cmake_path(RELATIVE_PATH CMAKE_CURRENT_SOURCE_DIR BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE COOL_PATH)
set(COOL_PATH_INTERNAL ${COOL_PATH} CACHE INTERNAL "") # Make a global variable so that `cool_setup()` will be able to use it when it is called outside of this CMake file

target_compile_definitions(Cool-PublicProperties INTERFACE
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
target_include_directories(Cool-PublicProperties INTERFACE src lib)
target_precompile_headers(Cool-PublicProperties INTERFACE # More info on precompiled headers: https://julesfouchy.github.io/Learn--Cpp-And-Dev-Practices/docs/lessons/precompiled-header
    <Cool/UDL/std.h> # Make std litterals available everywhere. E.g. "Hello"s will create a std::string, not a const char*.
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
    <fmt/format.h>
    <fmt/core.h>
    <fmt/std.h>
    <fmt/compile.h>
    <tl/expected.hpp>
    <Cool/Log/Debug.h>
    <Cool/Serialization/filesystem.h>
    <Cool/Serialization/img.h>
    <Cool/Serialization/GlmSerialization.h> # must be included after <glm/glm.hpp> because it depends on it
    <Cool/ostream/glm.h> # must be included after <glm/glm.hpp> because it depends on it
    <Cool/ostream/vector.h> # must be included after <vector> because it depends on it
)

if(COOL_USE_VULKAN)
    target_precompile_headers(Cool-PublicProperties INTERFACE
        <vulkan/vulkan.hpp>
        <vku/vku.hpp>
    )
elseif(COOL_USE_OPENGL)
    target_precompile_headers(Cool-PublicProperties INTERFACE
        <glad/gl.h>
        <Cool/Gpu/OpenGL/GLDebug.h>
    )
endif()

# -------------------------
# ---Add Vulkan / OpenGL---
# -------------------------
if(COOL_USE_VULKAN)
    target_compile_definitions(Cool-PublicProperties INTERFACE
        COOL_VULKAN
        COOL_VULKAN_VERSION=${COOL_USE_VULKAN})
    find_package(Vulkan REQUIRED)
    target_link_libraries(Cool-PublicProperties INTERFACE Vulkan::Vulkan)
    add_subdirectory(lib/Vookoo/include)
    target_link_libraries(Cool-PublicProperties INTERFACE Vookoo::Vookoo)
    add_subdirectory(lib/easy-shaderc)
    target_link_libraries(Cool-PublicProperties INTERFACE easy_shaderc::shaderc)
elseif(COOL_USE_OPENGL)
    target_compile_definitions(Cool-PublicProperties INTERFACE
        COOL_OPENGL
        COOL_OPENGL_VERSION=${COOL_USE_OPENGL})
    add_subdirectory(lib/glpp-extended)
    target_compile_definitions(glpp-extended PRIVATE $<$<CONFIG:Debug>:GLPP_SHOULD_CHECK_ERRORS>)
    target_link_libraries(Cool-PublicProperties INTERFACE glpp::extended)
endif()

# -------------------
# ---Add libraries---
# -------------------

# folder_watcher
add_subdirectory(lib/folder_watcher)
target_link_libraries(Cool-PublicProperties INTERFACE folder_watcher::folder_watcher)

# imgui_gradient
add_subdirectory(lib/imgui_gradient)
target_include_directories(imgui_gradient PRIVATE lib) # imgui_gradient needs acces to imgui
target_link_libraries(Cool-PublicProperties INTERFACE imgui_gradient::imgui_gradient)

# algorithms
add_subdirectory(lib/algorithms)
target_link_libraries(Cool-PublicProperties INTERFACE algorithms::algorithms)

# GLFW
add_subdirectory(lib/glfw)
target_link_libraries(Cool-PublicProperties INTERFACE glfw)

# cereal
target_include_directories(Cool-PublicProperties INTERFACE lib/cereal/include)

# smart
add_subdirectory(lib/smart)
target_link_libraries(Cool-PublicProperties INTERFACE smart::smart)

# Boxer
add_subdirectory(lib/Boxer)
target_link_libraries(Cool-PublicProperties INTERFACE Boxer)

# Native File Dialog
add_subdirectory(lib/nativefiledialog-extended EXCLUDE_FROM_ALL)
target_link_libraries(Cool-PublicProperties INTERFACE nfd)

# img
add_subdirectory(lib/img)
target_link_libraries(Cool-PublicProperties INTERFACE img::img)

# exe_path
add_subdirectory(lib/exe_path)
target_link_libraries(Cool-PublicProperties INTERFACE exe_path::exe_path)

# serv
add_subdirectory(lib/serv)
target_link_libraries(Cool-PublicProperties INTERFACE serv::serv)

# stringify
add_subdirectory(lib/stringify)
target_link_libraries(Cool-PublicProperties INTERFACE stringify::stringify)

# op
add_subdirectory(lib/op)
target_link_libraries(Cool-PublicProperties INTERFACE op::op)

# reg
add_subdirectory(lib/reg)
target_link_libraries(Cool-PublicProperties INTERFACE reg::reg)

# wafl
add_subdirectory(lib/wafl)
target_link_libraries(Cool-PublicProperties INTERFACE wafl::wafl)

# open_link
add_subdirectory(lib/open_link)
target_link_libraries(Cool-PublicProperties INTERFACE open_link::open_link)

# os_name
add_subdirectory(lib/os_name)
target_link_libraries(Cool-PublicProperties INTERFACE os_name::os_name)

# tl::expected
target_include_directories(Cool-PublicProperties INTERFACE lib/expected/include)

# Dear ImGui
add_subdirectory(lib/imgui)

# Build ImGui with glfw
target_link_libraries(ImGui PRIVATE glfw)
target_sources(ImGui PRIVATE lib/imgui/backends/imgui_impl_glfw.cpp)

# Build ImGui with Vulkan / OpenGL
if(COOL_USE_VULKAN)
    target_sources(ImGui PRIVATE lib/imgui/backends/imgui_impl_vulkan.cpp)
    target_link_libraries(ImGui PRIVATE Vulkan::Vulkan)
elseif(COOL_USE_OPENGL)
    target_sources(ImGui PRIVATE lib/imgui/backends/imgui_impl_opengl3.cpp)
endif()

target_link_libraries(Cool-PublicProperties INTERFACE ImGui::ImGui)

# ImNodes
add_library(ImNodes lib/imnodes/imnodes.cpp)
target_include_directories(ImNodes PRIVATE
    lib/imgui
    lib # To access imnodes_config.h
)
target_link_libraries(Cool-PublicProperties INTERFACE ImNodes)
target_link_libraries(ImNodes PUBLIC reg) # To define ImNode's ID as a reg::Id
target_compile_features(ImNodes PUBLIC cxx_std_11)
target_compile_definitions(ImNodes PUBLIC IMNODES_USER_CONFIG="imnodes_config.h")

# ----------------
# ---Copy files---
# ----------------
include("CMakeUtils/files_and_folders.cmake")
Cool__create_file_if_it_doesnt_exist(${CMAKE_SOURCE_DIR}/imgui.ini) # This prevents the following copy command from failing

function(cool_setup TARGET_NAME)
    Cool__target_copy_file(${TARGET_NAME} imgui.ini)
    Cool__target_copy_folder(${TARGET_NAME} ${COOL_PATH_INTERNAL}/res)
endfunction()

# ------------------
# ---Set the app icon---
# ------------------
# Call this function if you want to set an icon for your app.
# It will be used as the executable's icon and be visible in the corner of your app's window.
# To do so you will need both a .png and .ico files containing your logo.
#
# The .ico will only be used at compile time. For more info about .ico, see https://docs.fileformat.com/image/ico/.
# It needs to be in the same folder as a .rc file # containing "IDI_ICON1 ICON DISCARDABLE "your_icon.ico""
# where your_icon.ico is the name of your .ico file.
# `RC_FILE` is the path to that .rc file.
#
# The .png (or actually any image format supported by CoolLab) will be used at runtime.
# It needs to be copied alongside your executable using `Cool__target_copy_file` or `Cool__target_copy_folder`.
# `IMAGE_FILE` is the path to that .png file.
# It MUST be relative to the root of your project (a.k.a. to `Cool::Path::root()`).
function(Cool__set_app_icon
    TARGET
    RC_FILE
    IMAGE_FILE
)
    target_sources(${TARGET} PRIVATE ${RC_FILE})
    target_compile_definitions(Cool-PrivateProperties INTERFACE COOL_APP_ICON_FILE=\"${IMAGE_FILE}\")
endfunction()

# ---------------------
# ---Setup the tests---
# ---------------------
add_executable(Tests-Cool tests/tests.cpp ${COOL_SOURCES})
target_link_libraries(Tests-Cool PRIVATE Cool-PublicProperties)
target_link_libraries(Tests-Cool PRIVATE Cool-PrivateProperties)
target_compile_definitions(Tests-Cool PRIVATE COOL_ENABLE_TESTS)
add_subdirectory(lib/doctest)
target_link_libraries(Tests-Cool PRIVATE doctest::doctest)
