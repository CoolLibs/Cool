
add_compile_definitions(__COOL_ICONS)

include_directories(
    ${PATH_TO_COOL}/Icons/src
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PATH_TO_COOL}/Icons/icons
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Cool/Icons/icons
)