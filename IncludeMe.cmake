# NB : You need to set PATH_TO_COOL before including this file

add_compile_definitions(__COOL_APP)
add_compile_definitions(__COOL_GLM)

# OpenGL
find_package(OpenGL REQUIRED)

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
set(IMGUI_DIR ${PATH_TO_COOL}/App/lib/imgui)
add_library(IMGUI STATIC ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)

# glad
add_library(GLAD STATIC ${PATH_TO_COOL}/App/lib/glad/src/glad.c)

# Boxer
add_subdirectory(${PATH_TO_COOL}/App/lib/Boxer)

# Native File Dialog
add_subdirectory(${PATH_TO_COOL}/App/lib/nfd/src)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${IMGUI_DIR}
    ${PATH_TO_COOL}/App/lib/glad/include
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
    ${OPENGL_LIBRARIES}
    GLAD
    IMGUI
    Boxer
    nfd
    glfw
    Vulkan::Vulkan
)

# More infos on precompiled headers : https://www.youtube.com/watch?v=eSI4wctZUto&ab_channel=TheCherno
target_precompile_headers(${PROJECT_NAME} PRIVATE
    # libraries
    <imgui/imgui.h>
    <imgui/misc/cpp/imgui_stdlib.h>
    <glm/glm.hpp>
    <glm/gtc/type_ptr.hpp>
    <glad/glad.h>
    <boxer/boxer.h>
    <nfd.hpp>
    # Cool
    <Cool/App/internal/GLCall.h>
    # std
    <functional>
    <vector>
)