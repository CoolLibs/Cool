# cmake_minimum_required(VERSION 3.20)

# include("CMakeUtils/files_and_folders.cmake")

# function(target_link_opencv TARGET VISIBILITY)
#     set(RELEASE_OR_DEBUG "$<$<CONFIG:Debug>:d>")
    
#     #same include for all plateforms
#     set(OPENCV_FOLDER "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/opencv")
#     target_include_directories(${TARGET} SYSTEM ${VISIBILITY} "${OPENCV_FOLDER}/include")

#     if(WIN32)

#         set(OPENCV_BUILD_FOLDER "${OPENCV_FOLDER}/Windows/vc16")

#         target_link_libraries(${TARGET} ${VISIBILITY} "${OPENCV_BUILD_FOLDER}/lib/opencv_world470${RELEASE_OR_DEBUG}.lib")
        
#         # Copy all the .dll because they must be next to the executable.
#         Cool__target_copy_file(${TARGET}
#             "${OPENCV_BUILD_FOLDER}/bin/opencv_world470${RELEASE_OR_DEBUG}.dll"
#             "opencv_world470${RELEASE_OR_DEBUG}.dll"
#         )
#         Cool__target_copy_file(${TARGET}
#             "${OPENCV_BUILD_FOLDER}/bin/opencv_videoio_msmf470_64${RELEASE_OR_DEBUG}.dll"
#             "opencv_videoio_msmf470_64${RELEASE_OR_DEBUG}.dll"
#         )
#         Cool__target_copy_file(${TARGET}
#             "${OPENCV_BUILD_FOLDER}/bin/opencv_videoio_ffmpeg470_64.dll"
#             "opencv_videoio_ffmpeg470_64.dll"
#         )

#     elseif(UNIX) # for now, just the 4.5.5 version, only in Release
#         set(OPENCV_BUILD_FOLDER "${OPENCV_FOLDER}/Linux")
#         target_link_directories("${OPENCV_FOLDER}")
#         Cool__target_copy_file(${TARGET}
#             "${OPENCV_BUILD_FOLDER}/bin/libopencv_world.so"
#             "libopencv_world.so"
#         )

#     elseif(APPLE)

#     endif() 

# endfunction()

######################################################################################
######################################################################################
######################################################################################
######################################################################################
######################################################################################


cmake_minimum_required(VERSION 3.20)

include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeUtils/files_and_folders.cmake")

# function(opencv_folder)
#     return(PROPAGATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/opencv") # PROPAGATE ?
# endfunction()

# function(opencv_build_folder)
#     return(PROPAGATE opencv_folder() "/opencv") # PROPAGATE ?
# endfunction()

# set(OPENCV_FOLDER  "${OPENCV_FOLDER}"  CACHE INTERNAL opencv_folder())
# set(OPENCV_BUILD_FOLDER "${OPENCV_FOLDER}/Windows/vc16")


function(target_link_opencv_to_lib TARGET VISIBILITY)
    set(RELEASE_OR_DEBUG "$<$<CONFIG:Debug>:d>")

    #same include for all plateforms
    set(OPENCV_FOLDER "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/opencv")
    target_include_directories(${TARGET} SYSTEM ${VISIBILITY} "${OPENCV_FOLDER}/include")

    if(WIN32)

        set(OPENCV_BUILD_FOLDER "${OPENCV_FOLDER}/Windows/vc16")
        target_link_libraries(${TARGET} ${VISIBILITY} "${OPENCV_BUILD_FOLDER}/lib/opencv_world470${RELEASE_OR_DEBUG}.lib")
        

    elseif(UNIX) # for now, just the 4.5.5 version, only in Release
        set(OPENCV_BUILD_FOLDER "${OPENCV_FOLDER}/Linux")
        target_link_directories("${OPENCV_FOLDER}")

    elseif(APPLE)

    endif() 

endfunction()

function(target_link_opencv_to_exe TARGET VISIBILITY)
    set(RELEASE_OR_DEBUG "$<$<CONFIG:Debug>:d>")
    target_link_opencv_to_lib(${TARGET} ${VISIBILITY})

    if(WIN32)
        
    set(OPENCV_FOLDER "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/opencv") ## TODO(TD) don't do it twice, create a function
    set(OPENCV_BUILD_FOLDER "${OPENCV_FOLDER}/Windows/vc16")## TODO(TD) don't do it twice, create a function

        # Copy all the .dll because they must be next to the executable.
        Cool__target_copy_file(${TARGET}
            "${OPENCV_BUILD_FOLDER}/bin/opencv_world470${RELEASE_OR_DEBUG}.dll"
            "opencv_world470${RELEASE_OR_DEBUG}.dll"
        )
        Cool__target_copy_file(${TARGET}
            "${OPENCV_BUILD_FOLDER}/bin/opencv_videoio_msmf470_64${RELEASE_OR_DEBUG}.dll"
            "opencv_videoio_msmf470_64${RELEASE_OR_DEBUG}.dll"
        )
        Cool__target_copy_file(${TARGET}
            "${OPENCV_BUILD_FOLDER}/bin/opencv_videoio_ffmpeg470_64.dll"
            "opencv_videoio_ffmpeg470_64.dll"
        )

    elseif(UNIX) # for now, just the 4.5.5 version, only in Release
        Cool__target_copy_file(${TARGET}
            "${OPENCV_BUILD_FOLDER}/bin/libopencv_world.so"
            "libopencv_world.so"
        )

    elseif(APPLE)

    endif() 

endfunction()