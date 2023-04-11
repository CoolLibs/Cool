#pragma once

namespace Cool {

auto string_from_ratio(float ratio) -> std::string;

class AspectRatio {
public:
    AspectRatio() = default;
    explicit AspectRatio(float aspect_ratio);

    auto get() const -> float { return _ratio; }
    void set(float aspect_ratio);

    /// `width` is the size of the widget. Leave it at 0.f to get an automatic size.
    auto imgui(float width = 0.f) -> bool;

private:
    float       _ratio = 1.f;
    std::string _input{string_from_ratio(_ratio)};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    auto save_minimal(Archive const&) const -> float
    {
        return _ratio;
    }

    template<class Archive>
    void load_minimal(Archive const&, float const& value)
    {
        _ratio = value;
        _input = string_from_ratio(value);
    }
};

} // namespace Cool