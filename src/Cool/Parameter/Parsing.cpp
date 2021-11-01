#include "Parsing.h"
#include <Cool/String/String.h>

namespace Cool {

Parameter::AnyDesc parse_one_parameter_desc(std::string_view source)
{
    const auto body_pos = String::find_matching_pair(source, '{', '}');
    const auto type     = String::remove_whitespaces(source.substr(0, body_pos->first));
    const auto body     = "{\"\":" + std::string{source.substr(body_pos->first, body_pos->second - body_pos->first + 1)} + "}";
    const auto desc     = [&]() -> Parameter::AnyDesc {
        if (type == "float") {
            return parse<Parameter::FloatDesc>(body);
        }
        else if (type == "int") {
            return parse<Parameter::IntDesc>(body);
        }
        else if (type == "vec2") {
            return parse<Parameter::Vec2Desc>(body);
        }
        else if (type == "color") {
            return parse<Parameter::ColorDesc>(body);
        }
        else if (type == "direction3D") {
            return parse<Parameter::Dir3Desc>(body);
        }
        else {
            throw std::invalid_argument("'" + type + "' is not a valid parameter type. You can use 'int', 'float', 'vec2', 'color' or 'direction3D'");
        }
    }();
    return desc;
}

} // namespace Cool

TEST_CASE("[Cool::Parameter::Parsing] parse_one_parameter_desc()")
{
    const std::string source = R"(
float {
    "Name": "radius",
    "Default": 2.0,
    "Min": 3.0,
    "Max": 4.0
}
)";

    const auto param = Cool::parse_one_parameter_desc(source);
    REQUIRE(std::holds_alternative<Cool::Parameter::FloatDesc>(param));
    const auto p = std::get<Cool::Parameter::FloatDesc>(param);
    CHECK(p.name == "radius");
    CHECK(p.default_value == 2.f);
    CHECK(p.min_value == 3.f);
    CHECK(p.max_value == 4.f);
}