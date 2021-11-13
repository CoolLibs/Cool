function(internal_cool_copy FILE_OR_DIR COMMAND_NAME)
    string(SHA1 TARGET_HASH ${FILE_OR_DIR}) # Create a unique name for our target. We can't use ${FILE_OR_DIR} directly because it could contain invalid characters like spaces
    add_custom_target(CopyToOutput_${TARGET_HASH} ALL
        COMMENT "Copying \"${FILE_OR_DIR}\""
        COMMAND ${CMAKE_COMMAND} -E ${COMMAND_NAME}
            ${CMAKE_SOURCE_DIR}/${FILE_OR_DIR}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/${FILE_OR_DIR}
    )
endfunction()

function(copy_file_to_output FILENAME)
    internal_cool_copy(${FILENAME} copy)
endfunction()

function(copy_folder_to_output FOLDERNAME)
    internal_cool_copy(${FOLDERNAME} copy_directory)
endfunction()