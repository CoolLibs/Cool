#pragma once

namespace Cool {

class AspectRatio {
public:
    AspectRatio() = default;
    AspectRatio(float aspectRatio);
    ~AspectRatio() = default;

    inline float asFloat() const { return m_ratio; }

    bool ImGuiPicker(int uniqueID);

private:
    float m_ratio                 = 16.f / 9.f;
    int   m_ImGuiCurrentRatioItem = 0;
};

} // namespace Cool