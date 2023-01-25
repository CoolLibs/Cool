#pragma once

namespace Cool {

struct MathExpression {
    std::string expression{"x"};

    friend auto operator==(MathExpression const&, MathExpression const&) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    auto save_minimal(Archive const&) const -> std::string
    {
        return expression;
    }

    template<class Archive>
    void load_minimal(Archive const&, std::string const& value)
    {
        expression = value;
    }
};

inline auto to_string(MathExpression const& math_expression) -> std::string
{
    return fmt::format("\"{}\"", math_expression.expression);
}

auto imgui_widget(std::string_view name, MathExpression& expression) -> bool;

} // namespace Cool
