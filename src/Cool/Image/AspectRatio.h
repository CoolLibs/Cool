#pragma once

namespace Cool {

class AspectRatio {
public:
    AspectRatio() = default;
    AspectRatio(float aspect_ratio);

    auto as_float() const -> float { return _ratio; }

    auto imgui() -> bool;

private:
    float _ratio = 16.f / 9.f;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    float save_minimal(Archive const&) const
    {
        return _ratio;
    }

    template<class Archive>
    void load_minimal(Archive const&, float const& value)
    {
        _ratio = value;
    }
};

} // namespace Cool