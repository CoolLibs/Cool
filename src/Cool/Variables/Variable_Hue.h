#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/StrongTypes/Hue.h>
#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<Hue> {
    bool is_hdr = true;

    friend auto operator<=>(const VariableMetadata<Hue>&, const VariableMetadata<Hue>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<Hue>& var) -> bool
{
    return ImGuiExtras::hue_bar(
        var.name.c_str(),
        &var.value.value
        // Flags:
    );
}

inline auto imgui_widget(VariableMetadata<Hue>& meta) -> bool
{
    bool b = false;
    b |= ImGui::Checkbox("HDR", &meta.is_hdr);
    ImGui::SameLine();
    ImGuiExtras::help_marker("Allows the RGB values to go outside of the [0, 1] range.");
    return b;
}

} // namespace Cool
