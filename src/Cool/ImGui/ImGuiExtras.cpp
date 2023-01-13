#include "ImGuiExtras.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <Cool/Constants/Constants.h>
#include <Cool/File/File.h>
#include <Cool/Icons/Icons.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <open_link/open_link.hpp>
#include <ostream>
#include "Cool/Math/constants.h"

namespace Cool::ImGuiExtras {

void help_marker(const char* text)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.f);
        ImGui::TextUnformatted(text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

bool angle_wheel(const char* label, float* value_p, int number_of_snaps, float snaps_offset, bool always_snap)
{
    static constexpr float thickness = 2.0f;
    static constexpr float radius    = 25.0f;

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
    {
        return false;
    }
    //
    const ImGuiStyle& style       = ImGui::GetStyle();
    const float       line_height = ImGui::GetTextLineHeight();
    //
    ImVec2 p      = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(p.x + radius, p.y + radius);
    // Detect click
    ImGui::InvisibleButton(label, ImVec2(radius * 2.0f, radius * 2.0f));
    bool is_active  = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemHovered();

    if (is_active)
    {
        ImVec2 mp = ImGui::GetIO().MousePos;
        *value_p  = atan2f(center.y - mp.y, mp.x - center.x);
        if (always_snap || ImGui::GetIO().KeyShift)
        {
            const float slice = tau / static_cast<float>(number_of_snaps);
            *value_p          = std::floor((*value_p - snaps_offset) / slice + 0.5f) * slice + snaps_offset;
        }
    }

    float x2 = cosf(*value_p) * radius + center.x;
    float y2 = -sinf(*value_p) * radius + center.y;

    ImU32 col32 = ImGui::GetColorU32(
        is_active
            ? ImGuiCol_FrameBgActive
        : is_hovered
            ? ImGuiCol_FrameBgHovered
            : ImGuiCol_FrameBg
    );
    ImU32       col32line = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
    ImU32       col32text = ImGui::GetColorU32(ImGuiCol_Text);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddCircleFilled(center, radius, col32);
    draw_list->AddLine(center, ImVec2(x2, y2), col32line, thickness);
    draw_list->AddText(ImVec2(p.x + radius * 2.0f + style.ItemInnerSpacing.y, p.y + radius - line_height * 0.5f), col32text, label);

    const ImGuiID id = window->GetID(label);
    if (is_active)
    {
        ImGui::MarkItemEdited(id);
    }
    return is_active;
}

auto angle_slider(const char* label, float* value_p) -> bool
{
    float angle_in_deg = *value_p / tau;
    angle_in_deg       = angle_in_deg - std::floor(angle_in_deg);
    angle_in_deg *= 360.f;

    if (ImGui::SliderFloat(label, &angle_in_deg, 0.f, 360.f, "%.0f deg"))
    {
        *value_p = angle_in_deg * tau / 360.f;
        return true;
    }
    return false;
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
    if (total_duration == 0.f)
    {
        total_duration = time_in_sec;
    }
    auto t = static_cast<uint32_t>(time_in_sec);
    if (total_duration < 60.f)
    {
        ImGui::Text("%us", t);
    }
    else if (total_duration < 3600.f)
    {
        ImGui::Text("%um %02us", t / 60, t % 60);
    }
    else
    {
        ImGui::Text("%uh %02um %02us", t / 3600, (t % 3600) / 60, t % 60);
    }
}

void tooltip(const char* text)
{
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(text);
        ImGui::EndTooltip();
    }
}

void button_disabled(const char* label, const char* reason_for_disabling)
{
    maybe_disabled(true, reason_for_disabling, [&]() {
        ImGui::Button(label);
    });
}

auto colored_button(const char* label, float hue, const ImVec2& size) -> bool
{
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
    const auto is_clicked = ImGui::Button(label, size);
    ImGui::PopStyleColor(3);
    return is_clicked;
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

auto close_button() -> bool
{
    return button_with_icon(
        Icons::close_button().imgui_texture_id(),
        ImVec4(0.9f, 0.9f, 0.9f, 1.f),
        ImVec4(0.5f, 0.2f, 0.2f, 1.f),
        11.f, 11.f
    );
}

void image_framed(ImTextureID tex_id, const ImVec2& size, std::optional<float> frame_thickness, const ImVec4& frame_color, const ImVec4& background_color, const ImVec4& tint_color)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
    {
        return;
    }

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Default to using texture ID as ID. User can still push string/integer prefixes.
    // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
    ImGui::PushID(tex_id);
    const ImGuiID id = window->GetID("#image");
    ImGui::PopID();
    const ImVec2 padding = frame_thickness ? ImVec2(*frame_thickness, *frame_thickness) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id))
    {
        return;
    }

    // Render
    const ImU32 frameCol = frame_color.w > 0.0f ? ImGui::GetColorU32(frame_color) : ImGui::GetColorU32(ImGuiCol_Button);
    ImGui::RenderNavHighlight(bb, id);
    ImGui::RenderFrame(bb.Min, bb.Max, frameCol, true, ImClamp(ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    ImGui::RenderFrame(image_bb.Min, image_bb.Max, ImGui::GetColorU32(background_color), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    window->DrawList->AddImage(tex_id, image_bb.Min, image_bb.Max, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(tint_color));
}

bool input_uint(const char* label, unsigned int* value_p)
{
    return ImGui::InputScalar(label, ImGuiDataType_U32, value_p, nullptr, nullptr, "%u");
}

bool slider_uint32(const char* label, uint32_t* v, uint32_t v_min, uint32_t v_max, const char* format, float power)
{
    return ImGui::SliderScalar(label, ImGuiDataType_U32, v, &v_min, &v_max, format, power);
}

void warning_text(const char* text)
{
    ImGui::TextColored(Cool::Constants::imvec4_red, "%s", text);
}

bool begin_popup_context_menu_from_button(const char* label, ImGuiPopupFlags popup_flags)
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    if (window->SkipItems)
    {
        return false;
    }
    ImGuiID id = label ? window->GetID(label) : GImGui->LastItemData.ID; // If user hasn't passed an ID, we can use the LastItemID. Using LastItemID as a Popup ID won't conflict!
    IM_ASSERT(id != 0);                                                  // You cannot pass a NULL str_id if the last item has no identifier (e.g. a Text() item)
    if (ImGui::Button(label))
    {
        ImGui::OpenPopupEx(id, popup_flags);
    }
    return ImGui::BeginPopupEx(id, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);
}

void invisible_wrapper_around_previous_line(const char* str_id)
{
    ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, 2 * ImGui::GetTextLineHeight()));
    ImGui::InvisibleButton(str_id, ImVec2(ImGui::GetWindowWidth(), 2 * ImGui::GetTextLineHeight()));
}

auto folder_dialog_button(
    std::filesystem::path* out_path,
    std::filesystem::path  initial_folder
) -> bool
{
    if (!button_with_icon(Cool::Icons::folder().imgui_texture_id(), ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(0.1f, 0.1f, 0.1f, 1.f)))
        return false;

    NFD::UniquePath outPath;
    nfdresult_t     result = NFD::PickFolder(outPath, std::filesystem::absolute(initial_folder).string().c_str());
    if (result != NFD_OKAY)
        return false;

    *out_path = std::filesystem::path{outPath.get()};
    return true;
}

auto file_dialog_button(
    std::filesystem::path*              out_path,
    std::vector<nfdfilteritem_t> const& file_filters,
    std::filesystem::path               initial_folder
) -> bool
{
    if (!button_with_icon(Cool::Icons::folder().imgui_texture_id(), ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(0.1f, 0.1f, 0.1f, 1.f)))
        return false;

    NFD::UniquePath outPath;
    nfdresult_t     result = NFD::OpenDialog(
        outPath,
        file_filters.data(),
        static_cast<nfdfiltersize_t>(file_filters.size()),
        std::filesystem::absolute(initial_folder).string().c_str()
    );
    if (result != NFD_OKAY)
        return false;

    *out_path = std::filesystem::path{outPath.get()};
    return true;
}

template<typename Callable>
static auto folder_file_impl(const char* label, std::filesystem::path* path, bool show_dialog_button, Callable&& dialog_button) -> bool
{
    ImGui::PushID(path);
    bool b              = false;
    auto path_as_string = path->string();
    ImGui::TextUnformatted(label);
    if (show_dialog_button)
    {
        ImGui::SameLine();
        b |= dialog_button();
    }
    ImGui::SameLine();
    if (ImGui::InputText("", &path_as_string))
    {
        b     = true;
        *path = std::filesystem::path{path_as_string};
    }
    ImGui::PopID();
    return b;
}

auto folder(const char* label, std::filesystem::path* folder_path, bool show_dialog_button) -> bool
{
    return folder_file_impl(label, folder_path, show_dialog_button, [&]() {
        return ImGuiExtras::folder_dialog_button(folder_path, *folder_path);
    });
}

auto file(const char* label, std::filesystem::path* file_path, std::vector<nfdfilteritem_t> const& file_filters, std::filesystem::path initial_folder, bool show_dialog_button) -> bool
{
    return folder_file_impl(label, file_path, show_dialog_button, [&]() {
        return ImGuiExtras::file_dialog_button(file_path, file_filters, !initial_folder.empty() ? initial_folder : *file_path);
    });
}

auto file_and_folder(
    const char*                         label,
    std::filesystem::path*              path,
    std::vector<nfdfilteritem_t> const& file_filters
) -> bool
{
    bool b           = false;
    auto folder_path = File::without_file_name(*path);
    auto file_path   = File::file_name(*path);

    b |= file((label + " (file)"s).c_str(), &file_path, file_filters, folder_path);
    b |= folder((label + " (folder)"s).c_str(), &folder_path, false);

    if (b)
        *path = folder_path / file_path;

    return b;
}

void image_centered(ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
    ImGui::SetCursorPos((ImGui::GetWindowSize() + ImVec2{0.f, ImGui::GetCurrentWindow()->TitleBarHeight()} - size) * 0.5f);
    ImGui::Image(texture_id, size, uv0, uv1, tint_col, border_col);
}

auto checkbox_with_submenu(const char* label, bool* bool_p, std::function<bool()> submenu) -> bool
{
    ImGui::PushID(label);
    bool was_used = ImGui::Checkbox("##checkbox", bool_p);
    ImGui::PopID();
    ImGui::SameLine();
    if (*bool_p)
    {
        if (ImGui::BeginMenu(label))
        {
            was_used |= submenu();
            ImGui::EndMenu();
        }
    }
    else
    {
        ImGui::TextUnformatted(label);
    }
    return was_used;
}

void maybe_disabled(bool condition, const char* reason_to_disable, std::function<void()> widgets)
{
    if (condition)
    {
        ImGui::BeginGroup();
        ImGui::BeginDisabled(true);

        widgets();

        ImGui::EndDisabled();
        ImGui::EndGroup();
        ImGuiExtras::tooltip(reason_to_disable);
    }
    else
    {
        ImGui::BeginGroup();

        widgets();

        ImGui::EndGroup();
    }
}

auto hue_wheel(const char* label, float* hue, float radius) -> bool
{
    ImGuiContext&      g      = *GImGui;
    const ImGuiWindow& window = *ImGui::GetCurrentWindow();
    if (window.SkipItems)
    {
        return false;
    }

    ImDrawList&       draw_list = *window.DrawList;
    const ImGuiStyle& style     = g.Style;

    g.NextItemData.ClearFlags();

    ImGui::PushID(label);
    ImGui::BeginGroup();

    // Setup
    const ImVec2 widget_pos = ImGui::GetCursorScreenPos();

    const float wheel_thickness = radius * .5f;
    const float wheel_r_outer   = radius;
    const float wheel_r_inner   = wheel_r_outer - wheel_thickness;
    const auto  wheel_center    = ImVec2{
        widget_pos.x + wheel_r_outer,
        widget_pos.y + wheel_r_outer};

    bool        value_changed = false;
    const float initial_hue   = *hue;

    // Hue wheel
    ImGui::InvisibleButton("Hue", ImVec2(wheel_r_outer * 2.f + style.ItemInnerSpacing.x, wheel_r_outer * 2.f));
    if (ImGui::IsItemActive())
    {
        const ImVec2 initial_off   = g.IO.MouseClickedPos[0] - wheel_center;
        const ImVec2 current_off   = g.IO.MousePos - wheel_center;
        const float  initial_dist2 = ImLengthSqr(initial_off);
        if (initial_dist2 >= (wheel_r_inner - 1) * (wheel_r_inner - 1) && initial_dist2 <= (wheel_r_outer + 1) * (wheel_r_outer + 1))
        {
            // Interaction with Hue wheel
            *hue = ImAtan2(current_off.y, current_off.x) / IM_PI * 0.5f;
            if (*hue < 0.0f)
                *hue += 1.0f;
            value_changed = true;
        }
    }

    const int   style_alpha8    = IM_F32_TO_INT8_SAT(style.Alpha);
    const ImU32 col_white       = IM_COL32(255, 255, 255, style_alpha8);
    const ImU32 col_midgrey     = IM_COL32(128, 128, 128, style_alpha8);
    const ImU32 col_hues[6 + 1] = {IM_COL32(255, 0, 0, style_alpha8), IM_COL32(255, 255, 0, style_alpha8), IM_COL32(0, 255, 0, style_alpha8), IM_COL32(0, 255, 255, style_alpha8), IM_COL32(0, 0, 255, style_alpha8), IM_COL32(255, 0, 255, style_alpha8), IM_COL32(255, 0, 0, style_alpha8)};

    ImVec4 hue_color_f(1, 1, 1, 0);
    ImU32  hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);

    // Render Hue Wheel
    const float aeps            = 0.5f / wheel_r_outer; // Half a pixel arc length in radians (2pi cancels out).
    const int   segment_per_arc = ImMax(4, static_cast<int>(wheel_r_outer) / 12);
    for (int n = 0; n < 6; n++)
    {
        const float a0             = (n) / 6.0f * 2.0f * IM_PI - aeps;
        const float a1             = (n + 1.0f) / 6.0f * 2.0f * IM_PI + aeps;
        const int   vert_start_idx = draw_list.VtxBuffer.Size;
        draw_list.PathArcTo(wheel_center, (wheel_r_inner + wheel_r_outer) * 0.5f, a0, a1, segment_per_arc);
        draw_list.PathStroke(col_white, 0, wheel_thickness);
        const int vert_end_idx = draw_list.VtxBuffer.Size;

        // Paint colors over existing vertices
        ImVec2 gradient_p0(wheel_center.x + ImCos(a0) * wheel_r_inner, wheel_center.y + ImSin(a0) * wheel_r_inner);
        ImVec2 gradient_p1(wheel_center.x + ImCos(a1) * wheel_r_inner, wheel_center.y + ImSin(a1) * wheel_r_inner);
        ImGui::ShadeVertsLinearColorGradientKeepAlpha(&draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col_hues[n], col_hues[n + 1]);
    }

    // Render Cursor + preview on Hue Wheel
    float  cos_hue_angle = ImCos(*hue * 2.0f * IM_PI);
    float  sin_hue_angle = ImSin(*hue * 2.0f * IM_PI);
    ImVec2 hue_cursor_pos(wheel_center.x + cos_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f, wheel_center.y + sin_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f);
    float  hue_cursor_rad      = value_changed ? wheel_thickness * 0.65f : wheel_thickness * 0.55f;
    int    hue_cursor_segments = ImClamp(static_cast<int>(hue_cursor_rad / 1.4f), 9, 32) * 10;
    draw_list.AddCircleFilled(hue_cursor_pos, hue_cursor_rad, hue_color32, hue_cursor_segments);
    draw_list.AddCircle(hue_cursor_pos, hue_cursor_rad + 1, col_midgrey, hue_cursor_segments);
    draw_list.AddCircle(hue_cursor_pos, hue_cursor_rad, col_white, hue_cursor_segments);
    draw_list.AddText(ImVec2(wheel_center.x + wheel_r_outer + style.ItemInnerSpacing.x, wheel_center.y - style.ItemInnerSpacing.y), col_white, label);

    ImGui::EndGroup();

    if (initial_hue == *hue)
    {
        value_changed = false;
    }

    if (value_changed)
    {
        ImGui::MarkItemEdited(g.LastItemData.ID);
    }

    ImGui::PopID();

    return value_changed;
}

static auto ImU32_from_ImVec4(ImVec4 color) -> ImU32
{
    return ImGui::GetColorU32(IM_COL32(
        color.x * 255.f,
        color.y * 255.f,
        color.z * 255.f,
        color.w * 255.f
    ));
}

static auto highlight_color(float opacity) -> ImVec4
{
    auto col = ImGui::GetStyleColorVec4(ImGuiCol_NavHighlight);
    col.w    = opacity;
    return col;
}

void background(std::function<void()> widget, ImVec4 color)
{
    ImDrawList& draw_list = *ImGui::GetWindowDrawList();
    draw_list.ChannelsSplit(2);                                   // Allows us to draw the highlight rectangle behind the widget,
    draw_list.ChannelsSetCurrent(1);                              // even though the widget is drawn first.
    const auto rectangle_start_pos = ImGui::GetCursorScreenPos(); // We must draw them in that order because we need to know the height of the widget before drawing the rectangle.

    widget();

    const auto rectangle_end_pos = ImVec2(
        rectangle_start_pos.x + ImGui::GetContentRegionAvail().x,
        ImGui::GetCursorScreenPos().y
    );
    draw_list.ChannelsSetCurrent(0);
    draw_list.AddRectFilled(
        rectangle_start_pos,
        rectangle_end_pos,
        ImU32_from_ImVec4(color)
    );
    draw_list.ChannelsMerge();
}

void highlight(std::function<void()> widget, float opacity)
{
    background(
        widget,
        highlight_color(opacity)
    );
}

auto link(std::string_view url) -> bool
{
    return link(url, url);
}

auto link(std::string_view url, std::string_view label) -> bool
{
    const bool opened = ImGui::Selectable(label.data(), true);
    if (opened)
    {
        Cool::open_link(url.data());
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
    return opened;
}

void bring_attention_if(bool should_bring_attention, std::function<void()> widget)
{
    if (should_bring_attention)
    {
        ImGui::SetWindowToFront();
        ImGuiExtras::highlight(widget, 1.f);
        ImGui::SetScrollHereX(0.f);
        ImGui::SetScrollHereY(0.f);
    }
    else
    {
        widget();
    }
}

} // namespace Cool::ImGuiExtras