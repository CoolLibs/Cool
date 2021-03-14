
add_compile_definitions(__COOL_MULTITHREAD)

include_directories(
    ${PATH_TO_COOL}/MultiThread/src
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <thread>
    <functional>
    <vector>
    <deque>
)