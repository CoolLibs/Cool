cmake_minimum_required(VERSION 3.20)

add_library(Cool)
add_library(Cool::Core ALIAS Cool)
target_compile_features(Cool PUBLIC cxx_std_20)

# Set COOL_PATH as the path to Cool, relative to the main project's CMakeLists.txt. It will look like "third-party/Cool"
cmake_path(RELATIVE_PATH CMAKE_CURRENT_SOURCE_DIR BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE COOL_PATH)

target_compile_definitions(Cool PUBLIC
    $<$<CONFIG:Debug>:DEBUG> # Define DEBUG in debug mode
    COOL_PATH=\"${COOL_PATH}\" # This is useful to set the path to Cool's res directory
    $<$<CONFIG:Debug>:COOL_DEBUG_ONLY__CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"> # This is useful to set the current working directory in debug mode.
)

# -----------------------------
# ---Set include directories---
# -----------------------------

target_include_directories(Cool PUBLIC src)
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
    <doctest/doctest.h>
    <scope_guard/scope_guard.hpp>
    <Cool/Log/Log.h>
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
    include("Cool/cmake/vookoo.cmake") # TODO ?
    add_subdirectory(lib/shaderc-and-deps)
    target_link_libraries(Cool PUBLIC
        Vulkan::Vulkan # TODO ?
        shaderc
    )
elseif (COOL_USE_OPENGL)
    target_compile_definitions(Cool PUBLIC
        COOL_OPENGL
        COOL_OPENGL_VERSION=${COOL_USE_OPENGL})
    add_subdirectory(lib/glpp)
    target_link_libraries(Cool PUBLIC glpp::glpp)
endif()

# -------------------
# ---Add libraries---
# -------------------

# GLFW
set(GLFW_DIR lib/glfw)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
target_include_directories(Cool PUBLIC ${GLFW_DIR}/include)
target_include_directories(Cool PUBLIC ${GLFW_DIR}/deps)


target_link_libraries(Cool PUBLIC
    glfw
)


target_include_directories(Cool PUBLIC lib)

# Dear ImGui

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui)
add_library(IMGUI STATIC)

target_sources( IMGUI
                PRIVATE
                    ${IMGUI_DIR}/imgui_demo.cpp
                    ${IMGUI_DIR}/imgui_draw.cpp
                    ${IMGUI_DIR}/imgui_tables.cpp
                    ${IMGUI_DIR}/imgui_widgets.cpp
                    ${IMGUI_DIR}/imgui.cpp
                    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
                    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
                    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
                )

target_include_directories( IMGUI
                            PUBLIC ${IMGUI_DIR}
                            # PUBLIC ${IMGUI_DIR}/backends
                            # PUBLIC ${SDL2_DIR}/include
                            ${GLFW_DIR}/include
                            )




    # find_package(OpenGL REQUIRED)
    # target_link_libraries(IMGUI PUBLIC ${OPENGL_LIBRARIES})



# target_link_libraries(IMGUI PUBLIC SDL2main SDL2 ${CMAKE_DL_LIBS})




# file(GLOB_RECURSE IMGUI_SOURCES lib/imgui/*)
# if (COOL_USE_VULKAN)
#     list(REMOVE_ITEM IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp")
# endif()
# if (COOL_USE_OPENGL)
#     list(REMOVE_ITEM IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_vulkan.cpp")
# endif()
# add_library(IMGUI STATIC ${IMGUI_SOURCES})
# target_include_directories(IMGUI PUBLIC 
#     Cool/lib/imgui
#     Cool/lib/imgui/backends
#     Cool/lib/imgui/misc
# )
target_link_libraries(Cool PUBLIC IMGUI)

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
add_subdirectory(lib/nfd/src)
target_link_libraries(Cool PUBLIC nfd)
# Stb image
add_library(STB_IMAGE
    "lib/stb_image/stb_image.cpp"
    "lib/stb_image/stb_image_write.cpp")
target_link_libraries(Cool PRIVATE STB_IMAGE)
# doctest
add_subdirectory(lib/doctest)
target_link_libraries(Cool PUBLIC doctest::doctest)
target_compile_definitions(Cool PUBLIC $<$<NOT:$<CONFIG:Debug>>:DOCTEST_CONFIG_DISABLE>) # Strip tests from release builds

# ----------------------
# ---Add Cool sources---
# ----------------------

file(GLOB_RECURSE COOL_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(Cool PRIVATE ${COOL_SOURCES})

# ----------------
# ---Copy files---
# ----------------

file(APPEND ${CMAKE_SOURCE_DIR}/imgui.ini "") # Create imgui.ini if it doesn't exist. This prevents the following copy command from failing
include("cmake/copy_to_output.cmake")
Cool__copy_file_to_output(imgui.ini)
Cool__copy_folder_to_output(${COOL_PATH}/res)