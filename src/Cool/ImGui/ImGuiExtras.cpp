#include "ImGuiExtras.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <Cool/Constants/Constants.h>
#include <Cool/Icons/Icons.h>
#include <imgui/imgui_internal.h>

namespace Cool::ImGuiExtras {

void help_marker(const char* text)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.f);
        ImGui::TextUnformatted(text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

bool angle_wheel(const char* label, float* value_p, float thickness, float radius, int nb_segments_for_circle)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) {
        return false;
    }
    //
    const ImGuiStyle& style       = ImGui::GetStyle();
    const float       line_height = ImGui::GetTextLineHeight();
    //
    ImVec2 p      = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(p.x + radius, p.y + radius);
    // Detect clic
    ImGui::InvisibleButton(label, ImVec2(radius * 2.0f, radius * 2.0f));
    bool is_active  = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();

    if (is_active) {
        ImVec2 mp = ImGui::GetIO().MousePos;
        *value_p  = atan2f(center.y - mp.y, mp.x - center.x);
    }

    float x2 = cosf(*value_p) * radius + center.x;
    float y2 = -sinf(*value_p) * radius + center.y;

    ImU32       col32     = ImGui::GetColorU32(is_active    ? ImGuiCol_FrameBgActive
                                               : is_hovered ? ImGuiCol_FrameBgHovered
                                                            : ImGuiCol_FrameBg);
    ImU32       col32line = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
    ImU32       col32text = ImGui::GetColorU32(ImGuiCol_Text);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddCircleFilled(center, radius, col32, nb_segments_for_circle);
    draw_list->AddLine(center, ImVec2(x2, y2), col32line, thickness);
    draw_list->AddText(ImVec2(p.x + radius * 2.0f + style.ItemInnerSpacing.y, p.y + radius - line_height * 0.5f), col32text, label);

    const ImGuiID id = window->GetID(label);
    if (is_active) {
        ImGui::MarkItemEdited(id);
    }
    return is_active;
}

bool direction_3d(const char* label, float* value_p1, float* value_p2)
{
    ImGui::BeginGroup(); // Group the two wheels so that things like IsItemDeactivatedAfterEdit() work properly
    ImGui::PushID(label);
    bool b = false;

    ImGui::Text("%s :", label);
    b |= angle_wheel("Angle Ground", value_p1);
    b |= angle_wheel("Angle Up", value_p2);

    ImGui::PopID();
    ImGui::EndGroup();
    return b;
}

void time_formated_hms(float time_in_sec, float total_duration)
{
    if (total_duration == 0.f) {
        total_duration = time_in_sec;
    }
    auto t = static_cast<uint32_t>(time_in_sec);
    if (total_duration < 60.f) {
        ImGui::Text("%us", t);
    }
    else if (total_duration < 3600.f) {
        ImGui::Text("%um %02us", t / 60, t % 60);
    }
    else {
        ImGui::Text("%uh %02um %02us", t / 3600, (t % 3600) / 60, t % 60);
    }
}

void tooltip(const char* text)
{
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("%s", text);
        ImGui::EndTooltip();
    }
}

void button_disabled(const char* label, const char* reason_for_disabling)
{
    ImGui::BeginDisabled();
    ImGui::Button(label);
    ImGui::EndDisabled();
    invisible_wrapper_around_previous_line(reason_for_disabling);
    tooltip(reason_for_disabling);
}

bool button_with_icon(ImTextureID tex_id, const ImVec4& tint_color, const ImVec4& background_color, float button_width, float button_height, int frame_padding)
{
    return ImGui::ImageButton(tex_id, ImVec2(button_width, button_height), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), frame_padding, background_color, tint_color);
}

void button_with_icon_disabled(ImTextureID tex_id, const char* reason_for_disabling, float button_width, float button_height, std::optional<float> frame_padding)
{
    const ImVec4 grey = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
    image_framed(tex_id, ImVec2(button_width, button_height), frame_padding, grey, ImVec4(0.f, 0.f, 0.f, 1.f), grey);
    tooltip(reason_for_disabling);
}

void image_framed(ImTextureID tex_id, const ImVec2& size, std::optional<float> frame_thickness, const ImVec4& frame_color, const ImVec4& background_color, const ImVec4& tint_color)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) {
        return;
    }

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Default to using texture ID as ID. User can still push string/integer prefixes.
    // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
    ImGui::PushID((void*)(intptr_t)tex_id); // TODO
    const ImGuiID id = window->GetID("#image");
    ImGui::PopID();

    const ImVec2 padding = frame_thickness ? ImVec2(*frame_thickness, *frame_thickness) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id)) {
        return;
    }

    // Render
    const ImU32 frameCol = frame_color.w > 0.0f ? ImGui::GetColorU32(frame_color) : ImGui::GetColorU32(ImGuiCol_Button);
    ImGui::RenderNavHighlight(bb, id);
    ImGui::RenderFrame(bb.Min, bb.Max, frameCol, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding)); // TODO
    ImGui::RenderFrame(image_bb.Min, image_bb.Max, ImGui::GetColorU32(background_color), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    window->DrawList->AddImage(tex_id, image_bb.Min, image_bb.Max, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(tint_color));
}

bool input_uint(const char* label, unsigned int* value_p)
{
    return ImGui::InputScalar(label, ImGuiDataType_U32, value_p, nullptr, nullptr, "%u");
}

void warning_text(const char* text)
{
    ImGui::TextColored(Cool::Constants::imvec4_red, "%s", text);
}

bool begin_popup_context_menu_from_button(const char* label, ImGuiPopupFlags popup_flags)
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    if (window->SkipItems) {
        return false;
    }
    ImGuiID id = label ? window->GetID(label) : GImGui->LastItemData.ID; // If user hasn't passed an ID, we can use the LastItemID. Using LastItemID as a Popup ID won't conflict!
    IM_ASSERT(id != 0);                                                  // You cannot pass a NULL str_id if the last item has no identifier (e.g. a Text() item)
    if (ImGui::Button(label)) {
        ImGui::OpenPopupEx(id, popup_flags);
    }
    return ImGui::BeginPopupEx(id, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);
}

void invisible_wrapper_around_previous_line(const char* str_id)
{
    ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, 2 * ImGui::GetTextLineHeight()));
    ImGui::InvisibleButton(str_id, ImVec2(ImGui::GetWindowWidth(), 2 * ImGui::GetTextLineHeight()));
}

bool open_folder_dialog(std::string* out_path, std::string_view base_folder)
{
    if (button_with_icon(Cool::Icons::folder().imgui_texture_id(), ImVec4(1, 1, 1, 1), ImVec4(0.1, 0.1, 0.1, 1))) {
        NFD::UniquePath outPath;

        nfdresult_t result = NFD::PickFolder(outPath, std::filesystem::absolute(base_folder).string().c_str());
        if (result == NFD_OKAY) {
            *out_path = outPath.get();
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool open_file_dialog(std::string* out_path, std::vector<nfdfilteritem_t> file_type_filters, std::string_view base_folder)
{
    if (button_with_icon(Cool::Icons::folder().imgui_texture_id(), ImVec4(1, 1, 1, 1), ImVec4(0.1, 0.1, 0.1, 1))) {
        NFD::UniquePath outPath;
        // clang-format off
        nfdresult_t result = NFD::OpenDialog(
            outPath,
            file_type_filters.data(),
            file_type_filters.size(),
            std::filesystem::absolute(base_folder).string().c_str()
		);
        // clang-format on
        if (result == NFD_OKAY) {
            *out_path = outPath.get();
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

void image_centered(ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
    ImGui::SetCursorPos((ImGui::GetWindowSize() + ImVec2{0.f, ImGui::GetCurrentWindow()->TitleBarHeight()} - size) * 0.5f);
    ImGui::Image(texture_id, size, uv0, uv1, tint_col, border_col);
}

bool checkbox_with_submenu(const char* label, bool* bool_p, std::function<void()> submenu)
{
    ImGui::PushID(label);
    bool checkbox_was_used = ImGui::Checkbox("", bool_p);
    ImGui::PopID();
    ImGui::SameLine();
    if (*bool_p) {
        if (ImGui::BeginMenu(label)) {
            submenu();
            ImGui::EndMenu();
        }
    }
    else {
        ImGui::Text("%s", label);
    }
    return checkbox_was_used;
}

} // namespace Cool::ImGuiExtras