# NB : You need to set PATH_TO_COOL before including this file

find_package(OpenGL REQUIRED)

# On windows
if (WIN32) 
    set(SDL2_INCLUDE_DIRS ${PATH_TO_COOL}/lib/SDL2-2.0.10/include)
    set(SDL2_LIBRARIES ${PATH_TO_COOL}/lib/SDL2-2.0.10/lib/x64/SDL2.lib;
                       ${PATH_TO_COOL}/lib/SDL2-2.0.10/lib/x64/SDL2main.lib)
# On linux
else()
    find_package(SDL2 REQUIRED)
endif()

add_library(GLAD STATIC ${PATH_TO_COOL}/lib/glad/src/glad.c)
file(GLOB_RECURSE IMGUI_SOURCES ${PATH_TO_COOL}/lib/imgui/*)
add_library(IMGUI STATIC ${IMGUI_SOURCES})

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${PATH_TO_COOL}/lib/glad/include
    ${PATH_TO_COOL}/lib/imgui
    ${PATH_TO_COOL}/lib
    ${PATH_TO_COOL}/lib/glm
    ${PATH_TO_COOL}/lib/spdlog/include
    ${PATH_TO_COOL}/src
)

target_link_libraries(
    ${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES}
    GLAD
    IMGUI
)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} -ldl)
endif()

if (WIN32) # Copy .dll to output folder
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PATH_TO_COOL}/lib/SDL2-2.0.10/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Copy imgui.ini to output folder
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/imgui.ini
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/imgui.ini
)