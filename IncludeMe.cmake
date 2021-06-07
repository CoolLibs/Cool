
add_compile_definitions(__COOL_OPENGL)

include_directories(
    ${PATH_TO_COOL}/OpenGL/src
)


add_compile_definitions(__COOL_CAMERA)

include_directories(
    ${PATH_TO_COOL}/Camera/src
)


add_compile_definitions(__COOL_TIME)

include_directories(
    ${PATH_TO_COOL}/Time/src
)

add_compile_definitions(__COOL_IMGUI)

include_directories(
    ${PATH_TO_COOL}/ImGui/src
)

add_compile_definitions(__COOL_FILE)

include_directories(
    ${PATH_TO_COOL}/File/src
)

add_compile_definitions(__COOL_RANDOM)

include_directories(
    ${PATH_TO_COOL}/Random/src
)

# NB : You need to set PATH_TO_COOL before including this file

add_compile_definitions(__COOL_APP)
add_compile_definitions(__COOL_GLM)
add_compile_definitions(__COOL_IMGUI_LIB)

if (COOL_USE_VULKAN)
    add_compile_definitions(__COOL_APP_VULKAN)
    include("${PATH_TO_COOL}/CMake-Helpers/vulkan.cmake")
endif()
if (COOL_USE_OPENGL)
    add_compile_definitions(__COOL_APP_OPENGL)
    find_package(OpenGL REQUIRED)
    add_library(GLAD STATIC ${PATH_TO_COOL}/App/lib/glad/src/glad.c)
    include_directories(
        ${OPENGL_INCLUDE_DIR}
        ${PATH_TO_COOL}/App/lib/glad/include
    )
endif()

# GLFW
set(GLFW_DIR ${PATH_TO_COOL}/App/lib/glfw)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)
include_directories(${GLFW_DIR}/deps)

# Dear ImGui
file(GLOB_RECURSE IMGUI_SOURCES ${PATH_TO_COOL}/App/lib/imgui/*)
if (COOL_USE_VULKAN)
    list(REMOVE_ITEM IMGUI_SOURCES ${PATH_TO_COOL}/App/lib/imgui/backends/imgui_impl_opengl3.cpp)
endif()
if (COOL_USE_OPENGL)
    list(REMOVE_ITEM IMGUI_SOURCES ${PATH_TO_COOL}/App/lib/imgui/backends/imgui_impl_vulkan.cpp)
endif()
add_library(IMGUI STATIC ${IMGUI_SOURCES})

# Boxer
add_subdirectory(${PATH_TO_COOL}/App/lib/Boxer)

# Native File Dialog
add_subdirectory(${PATH_TO_COOL}/App/lib/nfd/src)

include_directories(
    ${PATH_TO_COOL}/App/lib/imgui
    ${PATH_TO_COOL}/App/lib/glm
    ${PATH_TO_COOL}/App/lib/nfd/src/include
    ${PATH_TO_COOL}/App/lib
    ${PATH_TO_COOL}/App/src
)

# Add a post build operation to copy IMGUI.ini to the output folder (where the executable is created)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/imgui.ini
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/imgui.ini
)

# Add libraries to the project
target_link_libraries(${PROJECT_NAME}
    IMGUI
    Boxer
    nfd
    glfw
)

if (COOL_USE_VULKAN)
    target_link_libraries(${PROJECT_NAME}
        Vulkan::Vulkan
    )
endif()
if (COOL_USE_OPENGL)
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        GLAD
    )
endif()

# More infos on precompiled headers : https://www.youtube.com/watch?v=eSI4wctZUto&ab_channel=TheCherno
target_precompile_headers(${PROJECT_NAME} PRIVATE
    <imgui/imgui.h>
    <imgui/misc/cpp/imgui_stdlib.h>
    <glm/glm.hpp>
    <glm/gtc/type_ptr.hpp>
    <boxer/boxer.h>
    <nfd.hpp>
    <functional>
    <vector>
)

if (COOL_USE_OPENGL)
    target_precompile_headers(${PROJECT_NAME} PRIVATE
        <glad/glad.h>
        <Cool/App/internal/GLDebug.h>
    )
endif()

add_compile_definitions(__COOL_RENDERER_FULLSCREEN)

include_directories(
    ${PATH_TO_COOL}/Renderer_Fullscreen/src
)

# Add a post build operation to copy fullscreen.vert to the output folder (where the executable is created)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${PATH_TO_COOL}/Renderer_Fullscreen/fullscreen.vert
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Cool/Renderer_Fullscreen/fullscreen.vert
)
