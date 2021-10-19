#pragma once

#include "Parameter.h"

namespace Cool::Parameter {

struct IntDesc {
    int min_value = 0;
    int max_value = 20;
};

class Int : public Internal::Parameter<int> {
public:
    Int(const ParameterDesc<int>& base_desc = {}, IntDesc desc = {})
        : Parameter{base_desc}, _desc{desc}
    {
    }

protected:
    bool imgui_widget() override
    {
        bool b = ImGui::SliderInt(name().c_str(), &_value, _desc.min_value, _desc.max_value);
        ImGui::PushID(this);
        if (ImGui::BeginPopupContextItem()) {
            ImGui::DragInt("", &_desc.min_value);
            ImGui::SameLine();
            ImGui::Text("to");
            ImGui::SameLine();
            ImGui::DragInt(" ", &_desc.max_value);
            ImGui::EndPopup();
        }
        ImGui::PopID();
        return b;
    }

private:
    IntDesc _desc;
};

} // namespace Cool::Parameter