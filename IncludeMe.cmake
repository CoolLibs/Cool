
add_compile_definitions(__COOL_FILEWATCHER)

include_directories(
    ${PATH_TO_COOL}/FileWatcher/src
)

# More infos on precompiled headers : https://www.youtube.com/watch?v=eSI4wctZUto&ab_channel=TheCherno
target_precompile_headers(${PROJECT_NAME} PRIVATE
    <functional>
)
