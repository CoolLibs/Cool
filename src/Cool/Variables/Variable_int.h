#pragma once

#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<int> {
    int min_value{0};
    int max_value{10};

    friend auto operator<=>(const VariableMetadata<int>&, const VariableMetadata<int>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Min Value", min_value),
                cereal::make_nvp("Max Value", max_value));
    }
};

inline auto imgui_widget(Variable<int>& var) -> bool
{
    return ImGui::SliderInt(var.name.c_str(),
                            &var.value,
                            var.metadata.min_value,
                            var.metadata.max_value);
}

inline auto imgui_widget(VariableMetadata<int>& meta) -> bool
{
    bool b = false;
    b |= ImGui::DragInt("", &meta.min_value);
    ImGui::SameLine();
    ImGui::Text("to");
    ImGui::SameLine();
    b |= ImGui::DragInt(" ", &meta.max_value);
    return b;
}

} // namespace Cool
