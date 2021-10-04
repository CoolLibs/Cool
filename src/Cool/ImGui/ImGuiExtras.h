#pragma once

namespace Cool::ImGuiExtras {

/**
 * @brief A small "?" that displays some text when hovered.
 * 
 * @param text 
 */
void help_marker(const char* text);

/**
 * @brief A widget to pick an angle (in radians), displayed as a direction on a wheel
 * 
 * @param label Name displayed alongside the widget
 * @param value_p A pointer to the variable that the widget should modify
 * @param thickness Thickness of the line indicating the direction
 * @param radius Radius of the wheel (a.k.a. size of the widget)
 * @param nb_segments_for_circle Number of segments used to render the wheel (which is actually a polygon with a huge number of sides)
 * @return true iff the widget was just used
 */
bool angle_wheel(const char* label, float* value_p, float thickness = 2.0f, float radius = 25.0f, int nb_segments_for_circle = 26);

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

/**
 * @brief Adds a button that opens a folder dialog.
 * 
 * @param out_path A pointer to the variable where the resulting path should be stored
 * @param base_folder The folder that the dialog window should open at. Leave blank for default (plateform-specific) behaviour
 * @return true iff the button was clicked AND out_path was modified (i.e. the dialog was not canceled)
 */
bool open_folder_dialog(std::string* out_path, std::string_view base_folder = "");

/**
 * @brief Adds a button that opens a file dialog.
 * 
 * @param out_path A pointer to the variable where the resulting path should be stored.
 * @param file_type_filters A set of filters for the file types that should be selectable. Something like { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } }. You can find predefined filters in the Cool/App/NfdFileFilter.h header
 * @param base_folder The folder that the dialog window should open at. Leave blank for default (plateform-specific) behaviour
 * @return true iff the button was clicked AND out_path was modified (i.e. the dialog was not canceled)
 */
bool open_file_dialog(std::string* out_path, std::vector<nfdfilteritem_t> file_type_filters = {}, std::string_view base_folder = "");

/**
 * @brief Equivalent to ImGui::Image except the image will be centered in the window
 */
void image_centered(ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 1), const ImVec2& uv1 = ImVec2(1, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));

/**
 * @brief A checkbox that, when ticked, displays a menu on the side
 * 
 * @param label 
 * @param bool_p 
 * @param submenu A function that calls the imgui widgets that should appear in the submenu 
 * @return true iff the checkbox was used this frame
 */
bool checkbox_with_submenu(const char* label, bool* bool_p, std::function<void()> submenu);

} // namespace Cool::ImGuiExtras