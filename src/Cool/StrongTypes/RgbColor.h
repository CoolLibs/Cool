#pragma once

#include <op/op.hpp>

namespace Cool {
struct RgbColor : public op::Addable<RgbColor>
    , public op::Subtractable<RgbColor>
    , public op::Multipliable<RgbColor>
    , public op::Scalable<RgbColor>
    , public op::EqualityComparable<RgbColor> {
    glm::vec3 value;
    constexpr RgbColor() = default;
    constexpr explicit RgbColor(glm::vec3 value)
        : value{value} {}

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("RGB", value));
    }
};

inline auto to_string(Cool::RgbColor color) -> std::string
{
    return to_string(color.value);
}

} // namespace Cool
