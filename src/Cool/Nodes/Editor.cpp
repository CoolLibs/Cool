#include "Editor.h"

namespace Cool::internal {

void SearchBarState::on_nodes_menu_open()
{
    _nodes_filter.clear();
    _should_be_focused = true;
}

auto SearchBarState::get_nodes_filter() const -> std::string const&
{
    return _nodes_filter;
}

auto SearchBarState::imgui_widget() -> bool
{
    if (_should_be_focused)
    {
        ImGui::SetKeyboardFocusHere();
        _should_be_focused = false;
    }
    ImGui::PushID(868686);
    bool const b = ImGui::InputText("Filter", &_nodes_filter, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::PopID();
    return b;
}

} // namespace Cool::internal