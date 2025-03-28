/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "style_generator_extras.py" and edit the "style_imgui_definitions" function there.
 * -----------------------------------------------------------------------------
 */

auto Style::imgui_floating_buttons_spacing() -> bool
{
    bool const b = ImGui::SliderFloat("Floating buttons spacing", (float*)&floating_buttons_spacing, 0.f, 20.f, "%.3f");

    ImGuiExtras::help_marker("Small buttons that overlap the View");

    return b;
}

auto Style::imgui_frame_padding() -> bool
{
    bool const b = ImGui::SliderFloat2("Frame padding", (float*)&frame_padding, 0.f, 20.f, "%.3f");

    return b;
}

auto Style::imgui_tab_bar_padding() -> bool
{
    bool const b = ImGui::SliderFloat2("Tab bar padding", (float*)&tab_bar_padding, 0.f, 20.f, "%.3f");

    return b;
}

auto Style::imgui_menu_bar_spacing() -> bool
{
    bool const b = ImGui::SliderFloat2("Menu bar spacing", (float*)&menu_bar_spacing, 0.f, 20.f, "%.3f");

    return b;
}

auto Style::imgui_node_title_vertical_alignment() -> bool
{
    bool const b = ImGui::SliderFloat("Node title vertical alignment", (float*)&node_title_vertical_alignment, 0.f, 1.f);

    return b;
}