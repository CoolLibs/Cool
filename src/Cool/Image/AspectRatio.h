#pragma once

namespace Cool {

class AspectRatio {
public:
    AspectRatio() = default;
    AspectRatio(float aspect_ratio);

    auto get() const -> float { return _ratio; }
    void set(float aspect_ratio);

    /// `width` is the size of the widget. Leave it at 0.f to get an automatic size.
    auto imgui(float width = 0.f) -> bool;

private:
    float _ratio = 1.f;

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