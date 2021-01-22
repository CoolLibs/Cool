
add_compile_definitions(__COOL_SERIALIZATION)

include_directories(
    ${PATH_TO_COOL}/Serialization/src
    ${PATH_TO_COOL}/Serialization/lib/cereal/include/
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <cereal/access.hpp>
    <cereal/types/vector.hpp>
    <cereal/types/string.hpp>
    <cereal/types/memory.hpp>
)