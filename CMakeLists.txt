cmake_minimum_required(VERSION 3.16)

add_library(Cool)
add_library(Cool:: ALIAS Cool)
target_compile_features(Cool PUBLIC cxx_std_20)

# Define DEBUG in debug mode
target_compile_definitions(Cool PUBLIC $<$<CONFIG:Debug>:DEBUG>)

# Define COOL_DEBUG_ONLY__CMAKE_SOURCE_DIR in debug mode. This is useful to set the current working directory in debug mode.
target_compile_definitions(Cool PRIVATE $<$<CONFIG:Debug>:COOL_DEBUG_ONLY__CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\">)

target_include_directories(Cool PUBLIC
    src
    lib
    lib/imgui
    lib/glm
    lib/nfd/src/include
    lib/cereal/include
    lib/spdlog/include
    lib/doctest
    lib/Vookoo/include
    lib/smart/src
)

# More infos on precompiled headers : https://www.youtube.com/watch?v=eSI4wctZUto&ab_channel=TheCherno
target_precompile_headers(Cool PUBLIC
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
if (COOL_USE_OPENGL)
    target_precompile_headers(Cool PUBLIC
        <glad/glad.h>
        <Cool/Gpu/OpenGL/GLDebug.h>
    )
endif()
if (COOL_USE_VULKAN)
    target_precompile_headers(Cool PUBLIC
        <vulkan/vulkan.hpp>
        <vku/vku.hpp>
    )
endif()

# Include Vulkan / OpenGL
if (COOL_USE_VULKAN)
    target_compile_definitions(Cool PUBLIC COOL_VULKAN)
    target_compile_definitions(Cool PUBLIC COOL_VULKAN_VERSION=${COOL_USE_VULKAN})
    include("Cool/cmake/vookoo.cmake")
endif()
if (COOL_USE_OPENGL)
    target_compile_definitions(Cool PUBLIC COOL_OPENGL)
    target_compile_definitions(Cool PUBLIC COOL_OPENGL_VERSION=${COOL_USE_OPENGL})
    find_package(OpenGL REQUIRED)
    add_library(GLAD STATIC lib/glad/src/glad.c)
    target_include_directories(GLAD PUBLIC lib/glad/include)
    target_include_directories(Cool PUBLIC
        ${OPENGL_INCLUDE_DIR}
        lib/glad/include
    )
endif()

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




    find_package(OpenGL REQUIRED)
    target_link_libraries(IMGUI PUBLIC ${OPENGL_LIBRARIES})



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

# Boxer
add_subdirectory(lib/Boxer)

# Native File Dialog
add_subdirectory(lib/nfd/src)

# Stb image
add_library(STB_IMAGE STATIC "lib/stb_image/stb_image.cpp")
target_link_libraries(Cool PUBLIC
    STB_IMAGE
)
add_library(STB_IMAGE_WRITE STATIC "lib/stb_image/stb_image_write.cpp")
target_link_libraries(Cool PUBLIC
    STB_IMAGE_WRITE
)

# doctest
# Strip tests from release builds
target_compile_definitions(Cool PUBLIC $<$<NOT:$<CONFIG:Debug>>:DOCTEST_CONFIG_DISABLE>)

# Add libraries to the project
target_link_libraries(Cool PUBLIC
    Boxer
    nfd
    glfw
)
if (COOL_USE_VULKAN)
    # shaderc
    add_subdirectory(lib/shaderc-and-deps)
    target_link_libraries(Cool PUBLIC
        Vulkan::Vulkan
        shaderc
    )
endif()
if (COOL_USE_OPENGL)
    target_link_libraries(Cool PUBLIC
        ${OPENGL_LIBRARIES}
        GLAD
    )
endif()

# ---Add Cool sources---
file(GLOB_RECURSE COOL_SOURCES src/*)
target_sources(Cool PRIVATE ${COOL_SOURCES})
# ------

file(APPEND ${CMAKE_SOURCE_DIR}/imgui.ini "") # Create imgui.ini it it doesn't exist. This prevents to following copy command from failing
include("cmake/copy_to_output.cmake")
copy_file_to_output(imgui.ini)
copy_folder_to_output(Cool/res)