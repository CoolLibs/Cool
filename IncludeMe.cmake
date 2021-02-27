# NB : You need to set PATH_TO_COOL before including this file

add_compile_definitions(__COOL_APP)
add_compile_definitions(__COOL_GLM)

# Include OpenGL
find_package(OpenGL REQUIRED)

# Include Glfw
add_subdirectory(${PATH_TO_COOL}/App/lib/glfw)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
target_link_libraries(${PROJECT_NAME} glfw)

# Include glad
add_library(GLAD STATIC ${PATH_TO_COOL}/App/lib/glad/src/glad.c)

# Include DearImGui
file(GLOB_RECURSE IMGUI_SOURCES ${PATH_TO_COOL}/App/lib/imgui/*)
add_library(IMGUI STATIC ${IMGUI_SOURCES})

#Include Boxer
add_subdirectory(${PATH_TO_COOL}/App/lib/Boxer)

#Include Native File Dialog
add_subdirectory(${PATH_TO_COOL}/App/lib/nfd/src)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${PATH_TO_COOL}/App/lib/glad/include
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
    ${OPENGL_LIBRARIES}
    GLAD
    IMGUI
    Boxer
    nfd
)

# More infos on precompiled headers : https://www.youtube.com/watch?v=eSI4wctZUto&ab_channel=TheCherno
target_precompile_headers(${PROJECT_NAME} PRIVATE
    # libraries
    <imgui/imgui.h>
    <imgui/misc/cpp/imgui_stdlib.h>
    <glm/glm.hpp>
    <glm/gtc/type_ptr.hpp>
    <glad/glad.h>
    <GLFW/glfw3.h>
    <boxer/boxer.h>
    <nfd.hpp>
    # Cool
    <Cool/App/internal/GLCall.h>
    # std
    <functional>
    <vector>
)