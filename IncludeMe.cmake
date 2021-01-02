
add_compile_definitions(__COOL_EXPORTER)

include_directories(
    ${PATH_TO_COOL}/Exporter/src
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <string>
)