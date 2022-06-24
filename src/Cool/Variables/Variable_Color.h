#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include "Variable.h"

namespace Cool {

struct Color {
    glm::vec3 rgb;

    friend auto operator==(const Color&, const Color&) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("RGB", rgb));
    }
};

template<>
struct VariableMetadata<Color> {
    bool is_hdr = true;

    friend auto operator<=>(const VariableMetadata<Color>&, const VariableMetadata<Color>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Is HDR", is_hdr));
    }
};

inline auto imgui_widget(Variable<Color>& var) -> bool
{
    return ImGui::ColorEdit3(var.name.c_str(),
                             glm::value_ptr(var.value.rgb),
                             ImGuiColorEditFlags_Float | (var.metadata.is_hdr
                                                              ? ImGuiColorEditFlags_HDR
                                                              : 0));
}

inline auto imgui_widget(VariableMetadata<Color>& meta) -> bool
{
    bool b = false;
    b |= ImGui::Checkbox("HDR", &meta.is_hdr);
    ImGui::SameLine();
    ImGuiExtras::help_marker("Allows the RGB values to go outside of the [0, 1] range.");
    return b;
}

} // namespace Cool
