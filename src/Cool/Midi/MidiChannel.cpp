#include "MidiChannel.h"
#include <imgui.h>

namespace Cool {

auto user_facing_string(MidiChannelKind kind) -> const char*
{
    switch (kind)
    {
    case MidiChannelKind::Slider:
        return "Slider / Knob / Fader";
    case MidiChannelKind::ButtonToggle:
        return "Button (toggle)";
    case MidiChannelKind::ButtonSelector:
        return "Button (last selected)";
    case MidiChannelKind::ButtonWhilePressed:
        return "Button (while pressed)";
    default:
    {
        assert(false);
        return "Unknown";
    }
    }
}

auto imgui_widget(const char* label, MidiChannelKind& kind) -> bool
{
    bool b = false;
    if (ImGui::BeginCombo(label, user_facing_string(kind)))
    {
        for (int i = 0; i < static_cast<int>(MidiChannelKind::COUNT); ++i)
        {
            auto const select_kind = static_cast<MidiChannelKind>(i);
            if (ImGui::Selectable(user_facing_string(select_kind), select_kind == kind))
            {
                kind = select_kind;
                b    = true;
            }
        }
        ImGui::EndCombo();
    }
    return b;
}

} // namespace Cool