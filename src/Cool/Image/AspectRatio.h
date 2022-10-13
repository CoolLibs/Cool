#pragma once

namespace Cool {

class AspectRatio {
public:
    AspectRatio() = default;
    AspectRatio(float aspect_ratio);

    auto as_float() const -> float { return _ratio; }

    auto imgui() -> bool;

private:
    float _ratio                    = 16.f / 9.f;
    int   _imgui_current_ratio_item = 0;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Ratio", _ratio),
            cereal::make_nvp("Imgui current ratio item", _imgui_current_ratio_item)
        );
    }
};

} // namespace Cool