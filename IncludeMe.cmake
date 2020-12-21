
add_library(STB_IMAGE_WRITE STATIC "${PATH_TO_COOL}/ExportImage/lib/stb_image/stb_image_write.cpp")

target_link_libraries(${PROJECT_NAME}
    STB_IMAGE_WRITE
)

include_directories(
    ${PATH_TO_COOL}/ExportImage/lib/
    ${PATH_TO_COOL}/ExportImage/src
)