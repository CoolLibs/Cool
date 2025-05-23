/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "style_generator_extras.py" and edit the "register_elements" function there.
 * -----------------------------------------------------------------------------
 */

config.register_element(ImStyleEd::Element{
    .name        = "Toggle",
    .get_color   = []() { return ImGuiExtras::GetStyle().toggle; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().toggle = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "ToggleHovered",
    .get_color   = []() { return ImGuiExtras::GetStyle().toggle_hovered; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().toggle_hovered = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "ToggleBg",
    .get_color   = []() { return ImGuiExtras::GetStyle().toggle_bg; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().toggle_bg = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "ToggleBgHovered",
    .get_color   = []() { return ImGuiExtras::GetStyle().toggle_bg_hovered; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().toggle_bg_hovered = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "CheckboxButton",
    .get_color   = []() { return ImGuiExtras::GetStyle().checkbox_button; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().checkbox_button = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "CheckboxButtonHovered",
    .get_color   = []() { return ImGuiExtras::GetStyle().checkbox_button_hovered; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().checkbox_button_hovered = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "CheckboxButtonActive",
    .get_color   = []() { return ImGuiExtras::GetStyle().checkbox_button_active; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().checkbox_button_active = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "FloatingButton",
    .get_color   = []() { return ImGuiExtras::GetStyle().floating_button; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().floating_button = color; },
    .description = "Small buttons that overlap the View",
});

config.register_element(ImStyleEd::Element{
    .name        = "FloatingButtonHovered",
    .get_color   = []() { return ImGuiExtras::GetStyle().floating_button_hovered; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().floating_button_hovered = color; },
    .description = "Small buttons that overlap the View",
});

config.register_element(ImStyleEd::Element{
    .name        = "FloatingButtonActive",
    .get_color   = []() { return ImGuiExtras::GetStyle().floating_button_active; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().floating_button_active = color; },
    .description = "Small buttons that overlap the View",
});

config.register_element(ImStyleEd::Element{
    .name        = "FloatingButtonEnabled",
    .get_color   = []() { return ImGuiExtras::GetStyle().floating_button_enabled; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().floating_button_enabled = color; },
    .description = "Small buttons that overlap the View\nThis is the color used when the button is used as a toggle and is currently on.",
});

config.register_element(ImStyleEd::Element{
    .name        = "FloatingButtonHoveredEnabled",
    .get_color   = []() { return ImGuiExtras::GetStyle().floating_button_hovered_enabled; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().floating_button_hovered_enabled = color; },
    .description = "Small buttons that overlap the View\nThis is the color used when the button is used as a toggle and is currently on.",
});

config.register_element(ImStyleEd::Element{
    .name        = "FloatingButtonActiveEnabled",
    .get_color   = []() { return ImGuiExtras::GetStyle().floating_button_active_enabled; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().floating_button_active_enabled = color; },
    .description = "Small buttons that overlap the View\nThis is the color used when the button is used as a toggle and is currently on.",
});

config.register_element(ImStyleEd::Element{
    .name        = "ButtonsSeparator",
    .get_color   = []() { return ImGuiExtras::GetStyle().buttons_separator; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().buttons_separator = color; },
    .description = "Separator used when we glue several buttons together without any spacing between them (e.g. the timeline buttons).",
});

config.register_element(ImStyleEd::Element{
    .name        = "HighlightItems",
    .get_color   = []() { return ImGuiExtras::GetStyle().highlight_items; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().highlight_items = color; },
    .description = "Used for example when hovering some error messages, to highlight the part of the UI that needs to be used in order to fix the error message. Use the \"Test Message Console\" debug option to send such an error message.",
});

config.register_element(ImStyleEd::Element{
    .name        = "GradientMark",
    .get_color   = []() { return ImGuiExtras::GetStyle().gradient_mark; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().gradient_mark = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "GradientMarkHovered",
    .get_color   = []() { return ImGuiExtras::GetStyle().gradient_mark_hovered; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().gradient_mark_hovered = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "GradientMarkSelected",
    .get_color   = []() { return ImGuiExtras::GetStyle().gradient_mark_selected; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().gradient_mark_selected = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "GradientMarkSelectedHovered",
    .get_color   = []() { return ImGuiExtras::GetStyle().gradient_mark_selected_hovered; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().gradient_mark_selected_hovered = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "NodeLink",
    .get_color   = []() { return ImGuiExtras::GetStyle().node_link; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().node_link = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "NodeLinkHovered",
    .get_color   = []() { return ImGuiExtras::GetStyle().node_link_hovered; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().node_link_hovered = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "UrlLink",
    .get_color   = []() { return ImGuiExtras::GetStyle().url_link; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().url_link = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "UrlLinkHovered",
    .get_color   = []() { return ImGuiExtras::GetStyle().url_link_hovered; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().url_link_hovered = color; },
    .description = "",
});

config.register_element(ImStyleEd::Element{
    .name        = "Frame node",
    .get_color   = []() { return ImGuiExtras::GetStyle().frame_node_color; },
    .set_color   = [](ImVec4 const& color) { ImGuiExtras::GetStyle().frame_node_color = color; },
    .description = "",
});
