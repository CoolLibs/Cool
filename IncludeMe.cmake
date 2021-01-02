
add_compile_definitions(__COOL_LOAD_IMAGE)

add_library(STB_IMAGE STATIC "${PATH_TO_COOL}/LoadImage/lib/stb_image/stb_image.cpp")

target_link_libraries(${PROJECT_NAME}
    STB_IMAGE
)

include_directories(
    ${PATH_TO_COOL}/LoadImage/lib/
    ${PATH_TO_COOL}/LoadImage/src
)