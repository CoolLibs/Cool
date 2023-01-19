#pragma once

#include <filesystem>
#include "Cool/StrongTypes/Color.h"

namespace Cool::ImGuiExtras {

/// A small "?" that displays some text when hovered.
void help_marker(const char* text);

/// A widget to pick an angle (in radians), displayed as a direction on a wheel
/// When SHIFT is pressed (or `always_snap` is set to true) the angle will snap to some precise angles. `number_of_snaps` controls the number of such angles, and `snaps_offset` will offset these.
/// `number_of_snaps` must be >= 1. `snaps_offset` is expressed in radians.
auto angle_wheel(const char* label, float* value_p, int number_of_snaps = 24, float snaps_offset = 0.f, bool always_snap = false) -> bool;

auto angle_slider(const char* label, float* value_p) -> bool;

bool direction_3d(const char* label, float* value_p1, float* value_p2);

/**
 * @brief Writes some ImGui text : time is displayed as [hours]:[minutes]:[seconds]. Hours and minutes might not be displayed if they are 0 for both timeInSec and totalDuration
 *
 * @param time_in_sec The time to be displayed, expressed in seconds
 * @param total_duration Optionnal : the total length of the time interval that time_in_sec is part of, expressed in seconds. (for example time_in_sec might be a timestamp in a video, and total_duration would be the duration of the video). It allows the formating to know whether it should write hours and minutes or not, in order for the display to be consistent accross the wole duration of total_duration. Leave as 0.f if you don't want or need this behaviour.
 */
void time_formated_hms(float time_in_sec, float total_duration = 0.f);

/**
 * @brief Displays some text on hover for the previously declared widget.
 *
 * @param text
 */
void tooltip(const char* text);

/**
 * @brief A greyed out button that you can't click on.
 *
 * @param label The text inside the button
 * @param reason_for_disabling An explanation that will be shown on hover
 */
void button_disabled(const char* label, const char* reason_for_disabling = "Currently disabled");

/// A button colored with the given `hue` (a number between 0 and 1).
auto colored_button(const char* label, float hue, const ImVec2& size = ImVec2(0, 0)) -> bool;

/**
 * @brief A button that uses an image instead of text
 *
 * @param tex_id OpenGL texture ID of the image
 * @param tint_color You can tint your image
 * @param background_color Color that will appear in places where your image is transparent
 * @param button_width
 * @param button_height
 * @param frame_padding
 * @return true iff the button was just pressed
 */
bool button_with_icon(ImTextureID tex_id, const ImVec4& tint_color = ImVec4(1, 1, 1, 1), const ImVec4& background_color = ImVec4(0, 0, 0, 1), float button_width = 18.f, float button_height = 18.f, int frame_padding = 1);

/**
 * @brief A button that uses an image instead of text, but greyed out and you wan't click on it.
 *
 * @param tex_id OpenGL texture ID of the image
 * @param reason_for_disabling An explanation that will be shown on hover
 * @param button_width
 * @param button_height
 * @param frame_padding
 */
void button_with_icon_disabled(ImTextureID tex_id, const char* reason_for_disabling = "Currently disabled", float button_width = 18.f, float button_height = 18.f, std::optional<float> frame_padding = 1.f);

/// Draws a clickable red cross.
auto close_button() -> bool;

/**
 * @brief Displays an image with a frame around it
 *
 * @param tex_id OpenGL texture ID of the image
 * @param size Size the image will be displayed at
 * @param frame_thickness Thickness of the frame. Leave as std::nullopt if you don't want a frame
 * @param frame_color Color of the frame
 * @param background_color Color that will appear in places where your image is transparent
 * @param tint_color You can tint your image
 */
void image_framed(ImTextureID tex_id, const ImVec2& size, std::optional<float> frame_thickness = std::nullopt, const ImVec4& frame_color = ImVec4(0, 0, 0, 0), const ImVec4& background_color = ImVec4(0, 0, 0, 1), const ImVec4& tint_color = ImVec4(1, 1, 1, 1));

/**
 * @brief Like ImGui::InputInt, but for an unsigned int
 *
 * @param label
 * @param value_p
 * @return
 */
bool input_uint(const char* label, unsigned int* value_p);

bool slider_uint32(const char* label, uint32_t* v, uint32_t v_min, uint32_t v_max, const char* format = NULL, float power = 1.0f);

/**
 * @brief Writes some red text
 *
 * @param text
 */
void warning_text(const char* text);

/**
 * @brief Opens a context menu when you click on the button. Usage is :
 * if (ImGui::begin_popup_context_menu_from_button("Text")) {
 * 	// some ImGui widgets;
 * 	ImGui::EndPopup();
 * }
 *
 * @param label The text displayed inside the button
 * @param popup_flags The ImGui flags of the context menu
 * @return true iff the popup is open
 */
bool begin_popup_context_menu_from_button(const char* label, ImGuiPopupFlags popup_flags = ImGuiPopupFlags_None);

/**
 * @brief Can be useful for example to open a context menu on a dropdown, which would fail without this wrapper.
 *
 * @param str_id A label that won't be displayed but is used as an ID
 */
void invisible_wrapper_around_previous_line(const char* str_id);

/// Adds a button that opens a folder dialog.
/// `initial_folder` is the folder that the dialog window should open at. Leave blank for default (plateform-specific) behaviour.
/// Returns true iff out_path was modified.
auto folder_dialog_button(
    std::filesystem::path* out_path,
    std::filesystem::path  initial_folder = ""
) -> bool;

/// Adds a button that opens a file dialog.
/// `file_filters` is a set of filters for the file types that should be selectable. Something like { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } }. You can find predefined filters in <Cool/NfdFileFilter/NfdFileFilter.h>.
/// `initial_folder` is the folder that the dialog window should open at. Leave blank for default (plateform-specific) behaviour.
/// Returns true iff out_path was modified.
auto file_dialog_button(
    std::filesystem::path*              out_path,
    std::vector<nfdfilteritem_t> const& file_filters   = {},
    std::filesystem::path               initial_folder = ""
) -> bool;

/// UI for a folder path. Creates a text input and a button to open a folder explorer.
auto folder(
    const char*            label,
    std::filesystem::path* folder_path,
    bool                   show_dialog_button = true
) -> bool;

/// UI for a file path. Creates a text input and a button to open a file explorer.
/// `file_filters` is a set of filters for the file types that should be selectable. Something like { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } }. You can find predefined filters in <Cool/NfdFileFilter/NfdFileFilter.h>.
/// `initial_folder` is the folder that the dialog window should open at. Leave empty to use `file_path` as the initial folder.
auto file(
    const char*                         label,
    std::filesystem::path*              file_path,
    std::vector<nfdfilteritem_t> const& file_filters       = {},
    std::filesystem::path               initial_folder     = {},
    bool                                show_dialog_button = true
) -> bool;

/// UI for a file path that shows the file and its folder on two separate lines.
/// `file_filters` is a set of filters for the file types that should be selectable. Something like { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } }. You can find predefined filters in <Cool/NfdFileFilter/NfdFileFilter.h>.
auto file_and_folder(
    const char*                         label,
    std::filesystem::path*              path,
    std::vector<nfdfilteritem_t> const& file_filters = {}
) -> bool;

/// Equivalent to ImGui::Image except the image will be centered in the window
void image_centered(ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 1), const ImVec2& uv1 = ImVec2(1, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));

/// A checkbox that, when ticked, displays a menu on the side.
/// `submenu` is a function that calls the imgui widgets that should appear in the submenu, and returns true iff one of these widgets returned true.
/// returns true iff the checkbox or a widget in the submenu was used this frame.
bool checkbox_with_submenu(const char* label, bool* bool_p, std::function<bool()> submenu);

/// Like ImGui::BeginDisabled() + ImGui::EndDisabled(), but adds a message on hover
void maybe_disabled(bool condition, const char* reason_to_disable, std::function<void()> widgets);

/// Hues are numbers from 0 to 1. 0 and 1 correspond to red.
auto hue_wheel(const char* label, float* hue, float radius = 25.f) -> bool;

/// Renders `widget` with a colored background.
/// `widget` must be a function that draws some ImGui widgets.
void background(std::function<void()> widget, ImVec4 color);

/// Renders `widget` with a highlighted background.
/// `widget` must be a function that draws some ImGui widgets.
void highlight(std::function<void()> widget, float opacity = 1.f);

/// Creates a clickable link that opens the given url in the user's default web browser.
auto link(std::string_view url) -> bool;

/// Creates a clickable link that opens the given url in the user's default web browser.
/// It will be rendered as `label`.
auto link(std::string_view url, std::string_view label) -> bool;

/// Brings attention to the given widget (highlight, bring window to front, scroll to right position, etc.).
/// `widget` must be a function that draws some ImGui widgets.
/// If `should_bring_attention` is false, just renders the widget as usual. This is a convenience that makes calling code nicer.
void bring_attention_if(bool should_bring_attention, std::function<void()> widget);

///
auto colored_collapsing_header(std::string_view name, Cool::Color const& color) -> bool;

} // namespace Cool::ImGuiExtras