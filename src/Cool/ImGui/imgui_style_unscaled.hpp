#pragma once
#include "imgui-node-editor/imgui_node_editor.h"
#include "imgui.h"

namespace Cool {

inline auto imgui_style_unscaled() -> ImGuiStyle&
{
    static auto instance = ImGuiStyle{};
    return instance;
}

inline auto imgui_nodes_style_unscaled() -> ax::NodeEditor::Style&
{
    static auto instance = ax::NodeEditor::Style{};
    return instance;
}

} // namespace Cool