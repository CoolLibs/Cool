#pragma once

#include "Parameter.h"

namespace Cool::Parameter {

struct FloatDesc {
    float min_value = 0.f;
    float max_value = 1.f;
};

class Float : public Internal::Parameter<float> {
public:
    explicit Float(const ParameterDesc<float>& base_desc = {}, FloatDesc desc = {})
        : Parameter{base_desc}, _desc{desc}
    {
    }

protected:
    bool imgui_widget() override
    {
        bool b = ImGui::SliderFloat(name().c_str(), &_value, _desc.min_value, _desc.max_value);
        ImGui::PushID(this);
        if (ImGui::BeginPopupContextItem()) {
            ImGui::DragFloat("", &_desc.min_value);
            ImGui::SameLine();
            ImGui::Text("to");
            ImGui::SameLine();
            ImGui::DragFloat(" ", &_desc.max_value);
            ImGui::EndPopup();
        }
        ImGui::PopID();
        return b;
    }

private:
    FloatDesc _desc;
};

} // namespace Cool::Parameter