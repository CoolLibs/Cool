#pragma once

namespace internal {

static void draw_border_widget(ImDrawList& draw_list, const ImVec2 vec1, const ImVec2 vec2, ImColor color)
{
    draw_list.AddRect(vec1, vec2, color, 1.f, ImDrawFlags_None, 2.f);
}

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

static void draw_uniform_square(ImDrawList& draw_list, const ImVec2 vec1, const ImVec2 vec2, ImColor color)
{
    draw_list.AddRectFilled(vec1, vec2, color, 1.0f, ImDrawFlags_Closed);
}

static void draw_triangle(ImDrawList& draw_list, const ImVec2 vec_triangle_up, const ImVec2 vec_triangle_down_left, const ImVec2 vec_triangle_down_right, ImColor color)
{
    draw_list.AddTriangleFilled(vec_triangle_up, vec_triangle_down_left, vec_triangle_down_right, color);
}

static void draw_background_mark(ImDrawList& draw_list, const ImVec2 pos, ImColor border_color, ImColor inside_border_color, const float arrow_border, const float offset)
{
    const float arrow_inside_border = arrow_border - offset;

    const auto arrow_border_x = ImVec2{arrow_border, 0.f};
    const auto arrow_border_y = ImVec2{0.f, arrow_border};

    const auto offset_y = ImVec2{0.f, offset};

    const auto arrow_inside_border_x = ImVec2{arrow_inside_border, 0.f};
    const auto arrow_inside_border_y = ImVec2{0.f, arrow_inside_border};

    draw_triangle(draw_list, pos - arrow_border_y, pos - arrow_border_x, pos + arrow_border_x, border_color);
    draw_uniform_square(draw_list, pos - arrow_border_x, pos + arrow_border_x + ImVec2{0.f, 2.f} * arrow_border_y, border_color);
    draw_uniform_square(draw_list, pos - arrow_inside_border_x + offset_y, pos + arrow_inside_border_x + ImVec2{0.f, 2.f} * arrow_inside_border_y + offset_y, inside_border_color);
}

static void arrow_selected(ImDrawList& draw_list, const ImVec2 pos, ImColor selected_color, const float arrow_inside_border, const float arrow_selected, const float offset)
{
    const auto offset_y = ImVec2{0.f, offset};

    const auto arrow_inside_border_x = ImVec2{arrow_inside_border, 0.f};
    const auto arrow_inside_border_y = ImVec2{0.f, arrow_inside_border};

    const auto arrow_selected_x = ImVec2{arrow_selected, 0.f};
    const auto arrow_selected_y = ImVec2{0.f, arrow_selected};

    draw_triangle(draw_list, pos - arrow_selected_y - offset_y, pos + offset_y - arrow_selected_x, pos + arrow_selected_x + offset_y, selected_color);
    draw_list.AddRect(pos - arrow_inside_border_x + offset_y, pos + arrow_inside_border_x + ImVec2{0.f, 2.f} * arrow_inside_border_y + offset_y, selected_color, 1.0f, ImDrawFlags_Closed);
}

static void mark_button(const ImVec2 vec, const float arrow_border)
{
    ImGui::SetCursorScreenPos(vec - ImVec2{arrow_border, 0.f});
    const ImVec2 button_size = {arrow_border * 2.f, arrow_border * 2.f};
    ImGui::InvisibleButton("mark", button_size);
}

static void draw_mark(ImDrawList& draw_list, const ImVec2 pos, ImColor border_color, ImColor inside_border_color, ImColor selected_color, ImColor mark_color, const float arrow_border, bool cond)
{
    const float offset = 1.f;

    internal::draw_background_mark(
        draw_list,
        pos,
        border_color, inside_border_color,
        arrow_border, offset
    );
    if (cond)
    {
        const float arrow_selected      = 4.f;
        const float arrow_inside_border = arrow_border - offset;

        internal::arrow_selected(
            draw_list,
            pos,
            selected_color,
            arrow_border, arrow_selected, offset
        );
    }

    const auto square_height   = 3.f;
    const auto square_height_x = ImVec2{square_height, 0.f};
    const auto square_height_y = ImVec2{0.f, square_height};
    internal::draw_uniform_square(
        draw_list,
        pos - square_height_x + square_height_y,
        pos + square_height_x + square_height_y * square_height_y,
        mark_color
    );
}

static void mark_button(ImDrawList& draw_list, const ImVec2 pos, ImColor border_color, ImColor inside_border_color, ImColor selected_color, ImColor mark_color, bool cond)
{
    const float arrow_border = 6.f;
    draw_mark(draw_list, pos, border_color, inside_border_color, selected_color, mark_color, arrow_border, cond);
    internal::mark_button(pos, arrow_border);
}

} // namespace internal