

#include "imgui_color_gradient.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <iterator>
#include "imgui_draw.h"

namespace Cool {

ImGradient::ImGradient()
{
    add_mark(ImGradientMark{0.f, ImVec4{0.f, 0.f, 0.f, 1.f}});
    add_mark(ImGradientMark{1.f, ImVec4{1.f, 1.f, 1.f, 1.f}});
}

ImVec4 ImGradient::get_color_at(float position) const
{
    return compute_color_at(RelativePosition{
        ImClamp(position, 0.f, 1.f)}); // TODO(ASG) Offer more options: repeat, mirror, clamp, etc.
}

ImVec4 ImGradient::compute_color_at(RelativePosition position) const
{
    const ImGradientMark* lower = nullptr;
    const ImGradientMark* upper = nullptr;
    for (const ImGradientMark& mark : m_marks._marks)
    {
        if (mark.position > position &&
            (!upper || mark.position < upper->position))
        {
            upper = &mark;
        }
        if (mark.position < position &&
            (!lower || mark.position > lower->position))
        {
            lower = &mark;
        }
    }
    if (!lower && !upper)
    {
        return ImVec4{0.f, 0.f, 0.f, 1.f};
    }
    else if (upper && !lower)
    {
        lower = upper;
    }
    else if (!upper && lower)
    {
        upper = lower;
    }

    if (upper == lower)
    {
        return upper->color;
    }
    else
    {
        float distance = upper->position.get() - lower->position.get();
        float mix      = (position.get() - lower->position.get()) / distance;

        // lerp
        return ImVec4{(1.f - mix), (1.f - mix), (1.f - mix), (1.f - mix)} * lower->color +
               ImVec4{mix, mix, mix, mix} * upper->color;
    }
}

namespace ImGuiExtras {

static void draw_gradient_bar(ImGradient& gradient, const ImVec2& bar_pos, float width, float height)
{
    ImDrawList& draw_list  = *ImGui::GetWindowDrawList();
    const float bar_bottom = bar_pos.y + height;

    internal::draw_bar_border(draw_list, bar_pos, ImVec2(bar_pos.x + width, bar_bottom), Cool::variables::border_color());
    internal::draw_background_if(draw_list, bar_pos, ImVec2(bar_pos.x + width, bar_bottom), Cool::variables::empty_backgroung_color(), gradient.get_list().empty());

    float current_starting_x = bar_pos.x;
    for (auto markIt = gradient.get_list().begin(); markIt != gradient.get_list().end(); ++markIt)
    {
        ImGradientMark& mark = *markIt;

        ImU32 colorBU32 = ImGui::ColorConvertFloat4ToU32(mark.color);
        ImU32 colorAU32 = (markIt != gradient.get_list().begin()) ? ImGui::ColorConvertFloat4ToU32(std::prev(markIt)->color) : colorBU32;

        const float from = current_starting_x;
        const float to   = bar_pos.x + mark.position.get() * width;
        if (mark.position != 0.f)
        {
            internal::draw_gradient(draw_list, ImVec2(from, bar_pos.y), ImVec2(to, bar_bottom), colorAU32, colorBU32);
        }
        current_starting_x = to;
    }

    if (!gradient.get_list().empty() && gradient.get_list().back().position != 1.f)
    {
        ImU32 colorBU32 = ImGui::ColorConvertFloat4ToU32(gradient.get_list().back().color);
        internal::draw_uniform_square(draw_list, ImVec2(current_starting_x, bar_pos.y), ImVec2(bar_pos.x + width, bar_bottom), colorBU32);
    }

    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y + height + 10.0f));
}

static void draw_gradient_marks(ImGradient& gradient, ImGradientMark*& dragging_mark, ImGradientMark*& selected_mark, const ImVec2& bar_pos, float width, float height)
{
    ImDrawList& draw_list = *ImGui::GetWindowDrawList();

    for (auto markIt = gradient.get_list().begin(); markIt != gradient.get_list().end(); ++markIt)
    {
        Cool::ImGradientMark& mark = *markIt;

        internal::mark_button(
            draw_list,
            bar_pos + ImVec2(mark.position.get() * width, height),
            ImGui::ColorConvertFloat4ToU32(mark.color),
            selected_mark == &mark
        );

        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                selected_mark = &mark;
                dragging_mark = &mark;
            }
            if (ImGui::IsMouseClicked(ImGuiPopupFlags_MouseButtonMiddle))
            {
                selected_mark = &mark;
            }
            if (ImGui::IsMouseDoubleClicked(ImGuiPopupFlags_MouseButtonLeft))
            {
                selected_mark = &mark;
                ImGui::OpenPopup("picker");
            }
        }
    }

    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y + height + 20.0f));
}

bool gradient_button(ImGradient& gradient)
{
    const ImVec2 widget_pos = ImGui::GetCursorScreenPos();
    const float  width      = ImMax(250.0f, ImGui::GetContentRegionAvail().x - 100.0f);
    const bool   clicked    = ImGui::InvisibleButton("gradient_bar", ImVec2(widget_pos.x + width, GRADIENT_BAR_WIDGET_HEIGHT));

    draw_gradient_bar(gradient, widget_pos, width, GRADIENT_BAR_WIDGET_HEIGHT);

    return clicked;
}

bool ImGradientWidget::gradient_editor(std::string_view name, float horizontal_margin, ImGuiColorEditFlags flags)
{
    ImGui::Text("%s", name.data());
    const float  width      = std::max(1.f, ImGui::GetContentRegionAvail().x - 2.f * horizontal_margin);
    bool         modified   = false;
    const ImVec2 bar_pos    = Cool::variables::bar_position(horizontal_margin);
    const float  bar_bottom = bar_pos.y + GRADIENT_BAR_EDITOR_HEIGHT;
    ImDrawList&  draw_list  = *ImGui::GetWindowDrawList();

    internal::draw_border_widget(
        draw_list,
        bar_pos - ImVec2(horizontal_margin, ImGui::CalcTextSize(name.data()).y * 1.5f),
        ImVec2(width + horizontal_margin * 3.f, bar_bottom + variables::button_size() * 3.f),
        variables::border_color()
    );

    ImGui::BeginGroup();
    ImGui::InvisibleButton("gradient_editor_bar", ImVec2(width, GRADIENT_BAR_EDITOR_HEIGHT));

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        const float  pos          = ImClamp((ImGui::GetIO().MousePos.x - bar_pos.x) / width, 0.f, 1.f);
        const ImVec4 new_mark_col = gradient.get_color_at(pos);
        selected_mark             = gradient.add_mark(ImGradientMark{pos, new_mark_col});
        ImGui::OpenPopup("picker");
    }

    draw_gradient_bar(gradient, bar_pos, width, GRADIENT_BAR_EDITOR_HEIGHT);
    draw_gradient_marks(gradient, dragging_mark, selected_mark, bar_pos, width, GRADIENT_BAR_EDITOR_HEIGHT);

    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left) && dragging_mark)
    {
        dragging_mark = nullptr;
    }

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && dragging_mark)
    {
        const float map = ImClamp((ImGui::GetIO().MousePos.x - bar_pos.x) / width, 0.f, 1.f);
        if (dragging_mark->position.get() != map)
        {
            dragging_mark->position.set(map);
            gradient.get_marks().sorted();
            modified = true;
        }

        float diffY = ImGui::GetIO().MousePos.y - bar_bottom;
        if (diffY >= GRADIENT_MARK_DELETE_DIFFY)
        {
            gradient.remove_mark(*dragging_mark); // TODO(ASG) hide it when dragging and remove it when released
            dragging_mark = nullptr;
            selected_mark = nullptr;
            modified      = true;
        }
    }
    ImGui::EndGroup();

    if (
        (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonMiddle) &&
         ImGui::IsItemHovered()) ||
        ImGui::Button("-", ImVec2(variables::button_size(), variables::button_size()))
    )
    {
        gradient.remove_mark(*selected_mark);
        selected_mark = nullptr;
        modified      = true;
    }
    Cool::ImGuiExtras::tooltip("Remove a mark by middle click on it\nor by dragging it down");

    ImGui::SameLine();
    if (selected_mark && ImGui::ColorEdit4("##picker1", reinterpret_cast<float*>(&selected_mark->color), ImGuiColorEditFlags_NoInputs | flags))
    {
        modified = true;
    }

    ImGui::PushItemWidth(width * .25f);
    ImGui::SameLine();
    if (selected_mark && ImGui::DragFloat("##3", &selected_mark->position.get(), 1.f / width, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
    {
        gradient.get_marks().sorted();
        modified = true;
    }

    if (ImGui::BeginPopup("picker") && selected_mark)
    {
        ImGui::SetNextItemWidth(variables::button_size() * 12.f);
        bool colorModified = ImGui::ColorPicker4("##picker2", reinterpret_cast<float*>(&selected_mark->color), flags);
        if (selected_mark && colorModified)
        {
            modified = true;
        }
        ImGui::EndPopup();
    }
    return modified;
}

} // namespace ImGuiExtras

}; // namespace Cool