#pragma once

namespace Cool {

struct MathExpression {
    std::string expression{"x"};

    friend auto operator==(const MathExpression&, const MathExpression&) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // Minimal split serialization
        // https://uscilab.github.io/cereal/serialization_functions.html
        archive(expression);
    }
};

inline auto to_string(const MathExpression& math_expression) -> std::string
{
    return "\"" + math_expression.expression  + "\"";
}

auto imgui_widget(std::string_view name, MathExpression& expression) -> bool;

} // namespace Cool
