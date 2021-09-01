#include "AspectRatio.h"

namespace Cool {

AspectRatio::AspectRatio(float aspectRatio)
    : m_ratio(aspectRatio), m_ImGuiCurrentRatioItem(-1)
{
}

bool AspectRatio::ImGuiPicker(int uniqueID)
{
    ImGui::PushID(uniqueID);
    bool bUsed = false;
    ImGui::PushID("__AspectRatioComboBox");
    if (ImGui::Combo("", &m_ImGuiCurrentRatioItem, " 16/9\0 3/2\0 4/3\0 1/1\0 9/16\0 2/3\0 3/4\0\0")) {
        switch (m_ImGuiCurrentRatioItem) {
        case 0:
            m_ratio = 16.f / 9.f;
            break;
        case 1:
            m_ratio = 3.f / 2.f;
            break;
        case 2:
            m_ratio = 4.f / 3.f;
            break;
        case 3:
            m_ratio = 1.f;
            break;
        case 4:
            m_ratio = 9.f / 16.f;
            break;
        case 5:
            m_ratio = 2.f / 3.f;
            break;
        case 6:
            m_ratio = 3.f / 4.f;
            break;
        }
        bUsed = true;
    }
    ImGui::PopID();
    ImGui::PushID("__AspectRatioSlider");
    if (ImGui::SliderFloat("", &m_ratio, 0.5f, 2.f)) {
        m_ImGuiCurrentRatioItem = -1;
        bUsed                   = true;
    }
    ImGui::PopID();
    ImGui::PopID();
    return bUsed;
}

} // namespace Cool