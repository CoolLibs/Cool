
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