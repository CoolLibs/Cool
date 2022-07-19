//
//  imgui_color_gradient.cpp
//  imgui extension
//
//  Created by David Gallardo on 11/06/16.

#include "imgui_color_gradient.h"
#include <iterator>

namespace Cool {
static constexpr float GRADIENT_BAR_WIDGET_HEIGHT = 25.f;
static constexpr float GRADIENT_BAR_EDITOR_HEIGHT = 40.f;
static constexpr float GRADIENT_MARK_DELETE_DIFFY = 40.f;

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

// void ImGradient::refreshCache()
// {
//     for (int i = 0; i < 256; ++i)
//     {
//         computeColorAt(i / 255.0f, &m_cachedValues[i * 3]);
//     }
// }

namespace ImGuiExtras {

static void draw_bar_border(ImDrawList& draw_list, const ImVec2 vec1, const ImVec2 vec2, ImColor color)
{
    const auto margin = ImVec2{2.f, 2.f};
    draw_list.AddRectFilled(vec1 - margin, vec2 + margin, color);
}

static void draw_background_if(ImDrawList& draw_list, const ImVec2 vec1, const ImVec2 vec2, ImColor color, bool condition)
{
    if (condition)
    {
        draw_list.AddRectFilled(vec1, vec2, color);
    }
}

static void draw_gradient(ImDrawList& draw_list, const ImVec2 vec1, const ImVec2 vec2, ImColor colorA, ImColor colorB)
{
    draw_list.AddRectFilledMultiColor(vec1, vec2, colorA, colorB, colorB, colorA);
}

static void draw_gradient_bar(ImGradient& gradient, const ImVec2& bar_pos, float max_width, float height)
{
    ImDrawList& draw_list  = *ImGui::GetWindowDrawList();
    const float bar_bottom = bar_pos.y + height;

    draw_bar_border(draw_list, bar_pos, ImVec2(bar_pos.x + max_width, bar_bottom), IM_COL32(100, 100, 100, 255));
    draw_background_if(draw_list, bar_pos, ImVec2(bar_pos.x + max_width, bar_bottom), IM_COL32(0, 0, 0, 255), gradient.get_list().empty());

    float current_starting_x = bar_pos.x;
    for (auto markIt = gradient.get_list().begin(); markIt != gradient.get_list().end(); ++markIt)
    {
        ImGradientMark& mark = *markIt;

        ImU32 colorBU32 = ImGui::ColorConvertFloat4ToU32(mark.color);
        ImU32 colorAU32;
        if (markIt != gradient.get_list().begin())
        {
            colorAU32 = ImGui::ColorConvertFloat4ToU32(std::prev(markIt)->color);
        }
        else
        {
            colorAU32 = colorBU32;
        }

        const float from = current_starting_x;
        const float to   = bar_pos.x + mark.position.get() * max_width;
        if (mark.position != 0.f)
        {
            draw_gradient(draw_list, ImVec2(from, bar_pos.y), ImVec2(to, bar_bottom), colorAU32, colorBU32);
        }
        current_starting_x = to;
    }

    if (!gradient.get_list().empty() && gradient.get_list().back().position != 1.f)
    {
        ImU32 colorBU32 = ImGui::ColorConvertFloat4ToU32(gradient.get_list().back().color);
        draw_gradient(draw_list, ImVec2(current_starting_x, bar_pos.y), ImVec2(bar_pos.x + max_width, bar_bottom), colorBU32, colorBU32);
    }

    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y + height + 10.0f));
}

static void draw_gradient_marks(ImGradient& gradient, ImGradientMark*& dragging_mark, ImGradientMark*& selected_mark, const ImVec2& bar_pos, float max_width, float height)
{
    const float bar_bottom = bar_pos.y + height;
    ImDrawList& draw_list  = *ImGui::GetWindowDrawList();

    for (auto markIt = gradient.get_list().begin(); markIt != gradient.get_list().end(); ++markIt)
    {
        ImGradientMark& mark = *markIt;

        if (!selected_mark)
        {
            selected_mark = &mark;
        }

        const float to = bar_pos.x + mark.position.get() * max_width;

        const ImU32 arrow_color               = IM_COL32(100, 100, 100, 255);
        const ImU32 arrow_inside_border_color = IM_COL32(0, 0, 0, 255);
        const float arrow_border              = 6.f;
        const float offset                    = 1.f;
        const float arrow_inside_border       = arrow_border - offset;

        draw_list.AddTriangleFilled(ImVec2(to, bar_pos.y + (height - arrow_border)), ImVec2(to - arrow_border, bar_bottom), ImVec2(to + arrow_border, bar_bottom), arrow_color);
        draw_list.AddRectFilled(ImVec2(to - arrow_border, bar_bottom), ImVec2(to + arrow_border, bar_pos.y + (height + 2.f * arrow_border)), arrow_color, 1.0f, ImDrawFlags_Closed);
        draw_list.AddRectFilled(ImVec2(to - arrow_inside_border, bar_pos.y + (height + offset)), ImVec2(to + arrow_inside_border, bar_pos.y + (height + 2.f * arrow_inside_border + offset)), arrow_inside_border_color, 1.0f, ImDrawFlags_Closed);

        if (selected_mark == &mark)
        {
            const ImU32 selected_color = IM_COL32(0, 255, 0, 255);
            const float arrow_selected = 4.f;
            draw_list.AddTriangleFilled(ImVec2(to, bar_pos.y + (height - arrow_selected - offset)), ImVec2(to - arrow_selected, bar_bottom + offset), ImVec2(to + arrow_selected, bar_bottom + offset), selected_color);
            draw_list.AddRect(ImVec2(to - arrow_inside_border, bar_pos.y + (height + offset)), ImVec2(to + arrow_inside_border, bar_pos.y + (height + 2.f * arrow_inside_border + offset)), selected_color, 1.0f, ImDrawFlags_Closed);
        }
        const float square_height = 3.f;
        draw_gradient(
            draw_list,
            ImVec2(to - square_height, bar_pos.y + (height + square_height)),
            ImVec2(to + square_height, bar_pos.y + (height + square_height * square_height)),
            ImGui::ColorConvertFloat4ToU32(mark.color),
            ImGui::ColorConvertFloat4ToU32(mark.color)
        );

        ImGui::SetCursorScreenPos(ImVec2(to - arrow_border, bar_bottom));
        const ImVec2 button_size = {arrow_border * 2.f, arrow_border * 2.f};
        ImGui::InvisibleButton("mark", button_size);

        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseClicked(0))
            {
                selected_mark = &mark;
                dragging_mark = &mark;
            }
        }
    }

    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y + height + 20.0f));
}

bool gradient_button(ImGradient& gradient)
{
    const ImVec2 widget_pos = ImGui::GetCursorScreenPos();
    const float  max_width  = ImMax(250.0f, ImGui::GetContentRegionAvail().x - 100.0f);
    const bool   clicked    = ImGui::InvisibleButton("gradient_bar", ImVec2(max_width, GRADIENT_BAR_WIDGET_HEIGHT));

    draw_gradient_bar(gradient, widget_pos, max_width, GRADIENT_BAR_WIDGET_HEIGHT);

    return clicked;
}

bool ImGradientWidget::gradient_editor()
{
    bool   modified = false;
    ImVec2 bar_pos  = ImGui::GetCursorScreenPos();
    bar_pos.x += 10.f;
    const float max_width  = ImGui::GetContentRegionAvail().x - 20.f;
    const float bar_bottom = bar_pos.y + GRADIENT_BAR_EDITOR_HEIGHT;

    ImGui::InvisibleButton("gradient_editor_bar", ImVec2(max_width, GRADIENT_BAR_EDITOR_HEIGHT));

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
        const float  pos        = (ImGui::GetIO().MousePos.x - bar_pos.x) / max_width;
        const ImVec4 newMarkCol = gradient.get_color_at(pos);
        gradient.add_mark(ImGradientMark{pos, newMarkCol});
    }

    draw_gradient_bar(gradient, bar_pos, max_width, GRADIENT_BAR_EDITOR_HEIGHT);
    draw_gradient_marks(gradient, dragging_mark, selected_mark, bar_pos, max_width, GRADIENT_BAR_EDITOR_HEIGHT);

    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left) && dragging_mark)
    {
        dragging_mark = nullptr;
    }

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && dragging_mark)
    {
        const float map = (ImGui::GetIO().MousePos.x - bar_pos.x) / max_width;
        if (dragging_mark->position.get() != map &&
            map >= 0.f &&
            map <= 1.f)
        {
            dragging_mark->position.set(map);
            gradient.get_marks().sorted();
            // gradient.refreshCache();
            modified = true;
        }

        float diffY = ImGui::GetIO().MousePos.y - bar_bottom;
        if (diffY >= GRADIENT_MARK_DELETE_DIFFY)
        {
            gradient.remove_mark(*dragging_mark);
            dragging_mark = nullptr;
            selected_mark = nullptr;
            modified      = true;
        }
    }

    if (!selected_mark && gradient.get_list().size() > 0)
    {
        selected_mark = &gradient.get_list().front();
    }

    if (selected_mark)
    {
        bool colorModified = ImGui::ColorPicker4("##1", reinterpret_cast<float*>(&selected_mark->color));
        if (selected_mark && colorModified)
        {
            modified = true;
            // gradient.refreshCache();
        }
    }
    return modified;
}

} // namespace ImGuiExtras

}; // namespace Cool