#pragma once

#include "Parameter.h"

namespace Cool::Parameter {

struct IntDesc {
    using Rep = int;
    using Out = int;

    std::string name;
    int         default_value = 0;
    int         min_value     = 0;
    int         max_value     = 20;

    int value(int rep) const { return rep; }

    bool imgui(Rep& value)
    {
        bool b = ImGui::SliderInt(name.c_str(), &value, min_value, max_value);
        ImGui::PushID(this);
        if (ImGui::BeginPopupContextItem()) {
            ImGui::DragInt("", &min_value);
            ImGui::SameLine();
            ImGui::Text("to");
            ImGui::SameLine();
            ImGui::DragInt(" ", &max_value);
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
                cereal::make_nvp("Default value", default_value),
                cereal::make_nvp("Min value", min_value),
                cereal::make_nvp("Max value", max_value));
    }
};

using Int = Parameter_Base<IntDesc>;

} // namespace Cool::Parameter