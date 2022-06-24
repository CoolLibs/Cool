#include "Parsing.h"
#include <Cool/String/String.h>

namespace Cool {

std::vector<Parameter::AnyDesc> parse_all_parameter_desc(std::string_view source)
{
    auto res       = std::vector<Parameter::AnyDesc>{};
    auto parse_res = parse_one_parameter_desc(source);
    while (parse_res.has_value())
    {
        res.push_back(parse_res->first);
        source    = source.substr(parse_res->second + 1, source.length() - parse_res->second);
        parse_res = parse_one_parameter_desc(source);
    }
    return res;
}

std::optional<std::pair<Parameter::AnyDesc, size_t>> parse_one_parameter_desc(std::string_view source)
{
    const auto body_pos = String::find_matching_pair(source, '{', '}');
    if (!body_pos.has_value())
    {
        return std::nullopt;
    }
    const auto type = String::remove_whitespaces(source.substr(0, body_pos->first));
    const auto body = "{\"\":" + std::string{source.substr(body_pos->first, body_pos->second - body_pos->first + 1)} + "}";
    const auto desc = [&]() -> Parameter::AnyDesc {
        if (type == "float")
        {
            return parse<Parameter::FloatDesc>(body);
        }
        else if (type == "int")
        {
            return parse<Parameter::IntDesc>(body);
        }
        else if (type == "bool")
        {
            return parse<Parameter::BoolDesc>(body);
        }
        else if (type == "vec2")
        {
            return parse<Parameter::Vec2Desc>(body);
        }
        else if (type == "vec3")
        {
            return parse<Parameter::Vec3Desc>(body);
        }
        else if (type == "color")
        {
            return parse<Parameter::ColorDesc>(body);
        }
        else if (type == "direction3D")
        {
            return parse<Parameter::Dir3Desc>(body);
        }
        else if (type == "rotation3D")
        {
            return parse<Parameter::Rot3Desc>(body);
        }
        else
        {
            throw std::invalid_argument("'" + type + "' is not a valid parameter type. You can use 'int', 'float', 'vec2', 'vec3', 'color' or 'direction3D'");
        }
    }();
    return std::make_pair(desc, body_pos->second);
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

    const auto parse_res = Cool::parse_one_parameter_desc(source);
    REQUIRE(parse_res.has_value());
    REQUIRE(std::holds_alternative<Cool::Parameter::FloatDesc>(parse_res->first));
    const auto param = std::get<Cool::Parameter::FloatDesc>(parse_res->first);
    CHECK(param.name == "radius");
    CHECK(param.default_value == 2.f);
    CHECK(param.min_value == 3.f);
    CHECK(param.max_value == 4.f);
}

TEST_CASE("[Cool::Parameter::Parsing] parse_one_parameter_desc()")
{
    const std::string source = R"(
float {
    "Name": "radius",
    "Default": 2.0,
    "Min": 3.0,
    "Max": 4.0
}

color {
    "Name": "test_color",
    "Default": {
        "x": 0.7,
        "y": 0.2,
        "z": 0.9
    }
})";

    const auto params_list = Cool::parse_all_parameter_desc(source);
    REQUIRE(params_list.size() == 2);
    REQUIRE(std::holds_alternative<Cool::Parameter::FloatDesc>(params_list[0]));
    REQUIRE(std::holds_alternative<Cool::Parameter::ColorDesc>(params_list[1]));
    const auto param2 = std::get<Cool::Parameter::ColorDesc>(params_list[1]);
    CHECK(param2.name == "test_color");
    CHECK(param2.default_value == glm::vec3{0.7f, 0.2f, 0.9f});
}