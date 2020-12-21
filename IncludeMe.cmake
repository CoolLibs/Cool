
include_directories(
    ${PATH_TO_COOL}/Log/lib/spdlog/include
    ${PATH_TO_COOL}/Log/src
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <Cool/Log/Log.h>
)