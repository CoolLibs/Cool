#include "StyleEditor.h"
#include <cereal/archives/json.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "Cool/Nodes/nodes_style_editor.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/Serialization.h"
#include "ImStyleEd/cereal_style.hpp"

namespace Cool {

StyleEditor::StyleEditor()
{
    std::ignore = Cool::Serialization::load<StyleEditor, cereal::JSONInputArchive>(*this, Cool::Path::user_data() / "style.json");
}

StyleEditor::~StyleEditor()
{
    Cool::Serialization::save<StyleEditor, cereal::JSONOutputArchive>(*this, Cool::Path::user_data() / "style.json");
}

void StyleEditor::imgui()
{
    ImGuiExtras::separator_text("Rendering");
    ImGui::SliderFloat("Disabled Alpha", &ImGui::GetStyle().DisabledAlpha, 0.f, 1.f);
    ImGuiExtras::help_marker("Additional alpha multiplier for disabled items (multiply over current value of Alpha).");

    ImGuiExtras::separator_text("Main");
    ImGui::SliderFloat2("WindowPadding", (float*)&ImGui::GetStyle().WindowPadding, 0.0f, 20.0f, "%.0f");
    ImGuiExtras::GetStyle().imgui_frame_padding();
    ImGuiExtras::GetStyle().imgui_tab_bar_padding();
    ImGuiExtras::GetStyle().imgui_menu_bar_spacing();
    ImGui::SliderFloat2("CellPadding", (float*)&ImGui::GetStyle().CellPadding, 0.0f, 20.0f, "%.0f");
    ImGui::SliderFloat2("ItemSpacing", (float*)&ImGui::GetStyle().ItemSpacing, 0.0f, 20.0f, "%.0f");
    ImGui::SliderFloat2("ItemInnerSpacing", (float*)&ImGui::GetStyle().ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
    ImGuiExtras::GetStyle().imgui_floating_buttons_spacing();
    ImGui::SliderFloat2("TouchExtraPadding", (float*)&ImGui::GetStyle().TouchExtraPadding, 0.0f, 10.0f, "%.0f");
    ImGui::SliderFloat("IndentSpacing", &ImGui::GetStyle().IndentSpacing, 0.0f, 30.0f, "%.0f");
    ImGui::SliderFloat("ScrollbarSize", &ImGui::GetStyle().ScrollbarSize, 1.0f, 20.0f, "%.0f");
    ImGui::SliderFloat("GrabMinSize", &ImGui::GetStyle().GrabMinSize, 1.0f, 20.0f, "%.0f");

    ImGuiExtras::separator_text("Borders");
    ImGui::SliderFloat("WindowBorderSize", &ImGui::GetStyle().WindowBorderSize, 0.0f, 1.0f, "%.0f");
    ImGui::SliderFloat("ChildBorderSize", &ImGui::GetStyle().ChildBorderSize, 0.0f, 1.0f, "%.0f");
    ImGui::SliderFloat("PopupBorderSize", &ImGui::GetStyle().PopupBorderSize, 0.0f, 1.0f, "%.0f");
    ImGui::SliderFloat("FrameBorderSize", &ImGui::GetStyle().FrameBorderSize, 0.0f, 1.0f, "%.0f");
    ImGui::SliderFloat("TabBorderSize", &ImGui::GetStyle().TabBorderSize, 0.0f, 1.0f, "%.0f");

    ImGuiExtras::separator_text("Rounding");
    ImGui::SliderFloat("WindowRounding", &ImGui::GetStyle().WindowRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("ChildRounding", &ImGui::GetStyle().ChildRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("FrameRounding", &ImGui::GetStyle().FrameRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("PopupRounding", &ImGui::GetStyle().PopupRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("ScrollbarRounding", &ImGui::GetStyle().ScrollbarRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("GrabRounding", &ImGui::GetStyle().GrabRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("TabRounding", &ImGui::GetStyle().TabRounding, 0.0f, 12.0f, "%.0f");

    ImGuiExtras::separator_text("Widgets");
    ImGui::SliderFloat2("WindowTitleAlign", (float*)&ImGui::GetStyle().WindowTitleAlign, 0.0f, 1.0f, "%.2f");
    int window_menu_button_position = ImGui::GetStyle().WindowMenuButtonPosition + 1;
    if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
        ImGui::GetStyle().WindowMenuButtonPosition = window_menu_button_position - 1;
    ImGui::Combo("ColorButtonPosition", (int*)&ImGui::GetStyle().ColorButtonPosition, "Left\0Right\0");
    ImGui::SliderFloat2("ButtonTextAlign", (float*)&ImGui::GetStyle().ButtonTextAlign, 0.0f, 1.0f, "%.2f");
    ImGuiExtras::help_marker("Alignment applies when a button is larger than its text content.");
    ImGui::SliderFloat2("SelectableTextAlign", (float*)&ImGui::GetStyle().SelectableTextAlign, 0.0f, 1.0f, "%.2f");
    ImGuiExtras::help_marker("Alignment applies when a selectable is larger than its text content.");
    ImGui::SliderFloat("SeparatorTextBorderSize", &ImGui::GetStyle().SeparatorTextBorderSize, 0.0f, 10.0f, "%.0f");
    ImGui::SliderFloat2("SeparatorTextAlign", (float*)&ImGui::GetStyle().SeparatorTextAlign, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat2("SeparatorTextPadding", (float*)&ImGui::GetStyle().SeparatorTextPadding, 0.0f, 40.0f, "%0.f");
    ImGui::SliderFloat("LogSliderDeadzone", &ImGui::GetStyle().LogSliderDeadzone, 0.0f, 12.0f, "%.0f");

    ImGuiExtras::separator_text("Nodes");
    nodes_style_editor();

    ImGuiExtras::separator_text("Misc");
    ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&ImGui::GetStyle().DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
    ImGuiExtras::help_marker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
}

} // namespace Cool