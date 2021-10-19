#pragma once

#include "Parameter.h"

namespace Cool::Parameter {

struct Vec2Desc {
    glm::vec2 min_value = glm::vec2{0.f};
    glm::vec2 max_value = glm::vec2{1.f};
};

class Vec2 : public Internal::Parameter<glm::vec2> {
public:
    Vec2(const ParameterDesc<glm::vec2>& base_desc = {}, Vec2Desc desc = {})
        : Parameter{base_desc}, _desc{desc}
    {
    }

protected:
    bool imgui_widget() override
    {
        ImGui::PushID(this + 34);
        ImGui::PushItemWidth(150);
        bool b = ImGui::SliderFloat("", &_value.x, _desc.min_value.x, _desc.max_value.x);
        ImGui::PopID();
        ImGui::SameLine();
        b |= ImGui::SliderFloat(name().c_str(), &_value.y, _desc.min_value.y, _desc.max_value.y);
        ImGui::PopItemWidth();
        //ImGui::PushID(this);
        //if (ImGui::BeginPopupContextItem()) {
        //	ImGui::DragFloat("", &_min_value);
        //	ImGui::SameLine();
        //	ImGui::Text("to");
        //	ImGui::SameLine();
        //	ImGui::DragFloat(" ", &_max_value);
        //	ImGui::EndPopup();
        //}
        //ImGui::PopID();
        return b;
    }

private:
    Vec2Desc _desc;
};

} // namespace Cool::Parameter