# NB: these functions require policy CMP0112 which was set to NEW in CMake 3.19 (https://cmake.org/cmake/help/latest/policy/CMP0112.html#policy:CMP0112)
# If you are using a version of CMake lower than 3.19 then you need to call `cmake_policy(SET CMP0112 NEW)` just after `cmake_minimum_required(...)` in your main CMakeLists.txt file
# Otherwise you will get a "Cyclic dependencies" error

function(internal_cool_copy FILE_OR_DIR COMMAND_NAME)
    string(SHA1 TARGET_HASH ${FILE_OR_DIR})      # Create a unique name for our target. We can't use ${FILE_OR_DIR} directly because it could contain invalid characters like spaces
    set(TARGET_NAME CopyToOutput_${TARGET_HASH}) #
    add_custom_target(${TARGET_NAME}
        COMMENT "Copying \"${FILE_OR_DIR}\""
        COMMAND ${CMAKE_COMMAND} -E ${COMMAND_NAME}
            ${CMAKE_SOURCE_DIR}/${FILE_OR_DIR}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/${FILE_OR_DIR}
    )
    add_dependencies(${PROJECT_NAME} ${TARGET_NAME}) # Mark our custom target as a dependency of the main project, so that whenever the main project is launched our custom target is triggered and files are copied
endfunction()

function(Cool__copy_file_to_output FILENAME)
    internal_cool_copy(${FILENAME} copy)
endfunction()

function(Cool__copy_folder_to_output FOLDERNAME)
    internal_cool_copy(${FOLDERNAME} copy_directory)
endfunction()