#pragma once

namespace Cool {

class AspectRatio {
public:
    AspectRatio() = default;
    AspectRatio(float aspectRatio);

    inline float asFloat() const { return m_ratio; }

    bool ImGuiPicker(int uniqueID);

private:
    float m_ratio                 = 16.f / 9.f;
    int   m_ImGuiCurrentRatioItem = 0;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Ratio", m_ratio),
            cereal::make_nvp("Imgui current ratio item", m_ImGuiCurrentRatioItem)
        );
    }
};

} // namespace Cool