function(copy_folder_to_the_output_folder _FOLDERPATH)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${_FOLDERPATH}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/${_FOLDERPATH}
    )
endfunction()