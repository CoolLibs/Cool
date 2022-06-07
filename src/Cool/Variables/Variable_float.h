#pragma once

#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<float> {
    float min_value{0.f};
    float max_value{10.f};

    friend auto operator<=>(const VariableMetadata<float>&, const VariableMetadata<float>&) = default;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Min Value", min_value),
                cereal::make_nvp("Max Value", max_value));
    }
};

inline auto imgui_widget(Variable<float>& var) -> bool
{
    return ImGui::SliderFloat(var.name.c_str(),
                              &var.value,
                              var.metadata.min_value,
                              var.metadata.max_value);
}

inline auto imgui_widget(VariableMetadata<float>& meta) -> bool
{
    bool b = false;
    b |= ImGui::DragFloat("", &meta.min_value);
    ImGui::SameLine();
    ImGui::Text("to");
    ImGui::SameLine();
    b |= ImGui::DragFloat(" ", &meta.max_value);
    return b;
}

} // namespace Cool
