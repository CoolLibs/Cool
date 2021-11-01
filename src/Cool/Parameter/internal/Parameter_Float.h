#pragma once

#include "Parameter_Base.h"

namespace Cool::Parameter {

struct FloatDesc {
    using Value         = float;
    using InternalValue = float;

    std::string name;
    float       default_value = 0.f;
    float       min_value     = 0.f;
    float       max_value     = 1.f;

    float value(float rep) const { return rep; }

    bool imgui(float& value)
    {
        bool b = ImGui::SliderFloat(name.c_str(), &value, min_value, max_value);
        ImGui::PushID(this);
        if (ImGui::BeginPopupContextItem()) {
            ImGui::DragFloat("", &min_value);
            ImGui::SameLine();
            ImGui::Text("to");
            ImGui::SameLine();
            ImGui::DragFloat(" ", &max_value);
            ImGui::EndPopup();
        }
        ImGui::PopID();
        return b;
    }

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Name", name),
                cereal::make_nvp("Default", default_value),
                cereal::make_nvp("Min", min_value),
                cereal::make_nvp("Max", max_value));
    }
};

using Float = Parameter_Base<FloatDesc>;

} // namespace Cool::Parameter