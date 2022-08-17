#include <doctest/doctest.h>
#include "type_from_string.h"

static_assert(
    std::is_same_v<
        Cool::type_from_string<"int">,
        int>
);

static_assert(
    std::is_same_v<
        Cool::type_from_string<"bool">,
        bool>
);

template<typename T>
auto say_type() -> std::string
{
    return "FAILED";
}
template<>
auto say_type<int>() -> std::string
{
    return "I say INT";
}
template<>
auto say_type<float>() -> std::string
{
    return "I say FLOAT";
}

TEST_CASE("At runtime")
{
    std::string type_as_string = "int";
    CHECK(
        COOL_TFS_EVALUATE_FUNCTION_TEMPLATE(say_type, type_as_string, std::string, ()) ==
        "I say INT"
    );
}