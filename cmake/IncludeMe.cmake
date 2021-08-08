
# Define DEBUG in debug mode
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)
# Define CMAKE_SOURCE_DIR in debug mode. This is useful to set the current working directory in debug mode.
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:"CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"">)

include_directories(
    Cool/src
    Cool/lib
    Cool/lib/imgui
    Cool/lib/glm
    Cool/lib/nfd/src/include
    Cool/lib/cereal/include
    Cool/lib/spdlog/include
    Cool/lib/doctest
    Cool/lib/Vookoo/include
)

# More infos on precompiled headers : https://www.youtube.com/watch?v=eSI4wctZUto&ab_channel=TheCherno
target_precompile_headers(${PROJECT_NAME} PRIVATE
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
    
    <vulkan/vulkan.hpp>
    <vku/vku.hpp>
    
    <doctest/doctest.h>
    <Cool/Log/Log.h>
    <Cool/Serialization/internal/GlmSerialization.h> # must be included after <glm/glm.hpp> because it depends on it
    <Cool/glm/ostream.h>                             # must be included after <glm/glm.hpp> because it depends on it
)
if (COOL_USE_OPENGL)
    target_precompile_headers(${PROJECT_NAME} PRIVATE
        <glad/glad.h>
        <Cool/App/internal/GLDebug.h>
    )
endif()

# Include Vulkan / OpenGL
if (COOL_USE_VULKAN)
    add_compile_definitions(__COOL_APP_VULKAN)
    include("Cool/cmake/vookoo.cmake")
endif()
if (COOL_USE_OPENGL)
    add_compile_definitions(__COOL_APP_OPENGL)
    find_package(OpenGL REQUIRED)
    add_library(GLAD STATIC Cool/lib/glad/src/glad.c)
    include_directories(
        ${OPENGL_INCLUDE_DIR}
        Cool/lib/glad/include
    )
endif()

# GLFW
set(GLFW_DIR Cool/lib/glfw)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)
include_directories(${GLFW_DIR}/deps)

# Dear ImGui
file(GLOB_RECURSE IMGUI_SOURCES Cool/lib/imgui/*)
if (COOL_USE_VULKAN)
    list(REMOVE_ITEM IMGUI_SOURCES "${CMAKE_SOURCE_DIR}/Cool/lib/imgui/backends/imgui_impl_opengl3.cpp")
endif()
if (COOL_USE_OPENGL)
    list(REMOVE_ITEM IMGUI_SOURCES "${CMAKE_SOURCE_DIR}/Cool/lib/imgui/backends/imgui_impl_vulkan.cpp")
endif()
add_library(IMGUI STATIC ${IMGUI_SOURCES})

# Boxer
add_subdirectory(Cool/lib/Boxer)

# Native File Dialog
add_subdirectory(Cool/lib/nfd/src)

# Stb image
add_library(STB_IMAGE STATIC "Cool/lib/stb_image/stb_image.cpp")
target_link_libraries(${PROJECT_NAME}
    STB_IMAGE
)
add_library(STB_IMAGE_WRITE STATIC "Cool/lib/stb_image/stb_image_write.cpp")
target_link_libraries(${PROJECT_NAME}
    STB_IMAGE_WRITE
)

# doctest
# Strip tests from release builds
add_compile_definitions($<$<NOT:$<CONFIG:Debug>>:DOCTEST_CONFIG_DISABLE>)

# Add libraries to the project
target_link_libraries(${PROJECT_NAME}
    IMGUI
    Boxer
    nfd
    glfw
)
if (COOL_USE_VULKAN)
    # shaderc
    add_subdirectory(Cool/lib/shaderc-and-deps)
    target_link_libraries(${PROJECT_NAME}
        Vulkan::Vulkan
        shaderc
    )
endif()
if (COOL_USE_OPENGL)
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        GLAD
    )
endif()

# Post build : copy some files and folder to the output folder (where the executable is generated)
# imgui.ini 
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/imgui.ini
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/imgui.ini
)
# res
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Cool/res
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Cool/res
)