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

static void draw_background_mark(ImDrawList& draw_list, const ImVec2 vec_triangle_up, const ImVec2 vec_triangle_down_left, const ImVec2 vec_triangle_down_right, ImVec2 square_vec1, ImVec2 square_vec2, ImVec2 square_vec3, ImVec2 square_vec4, ImColor border_color, ImColor inside_border_color)
{
    draw_triangle(draw_list, vec_triangle_up, vec_triangle_down_left, vec_triangle_down_right, border_color);
    draw_uniform_square(draw_list, square_vec1, square_vec2, border_color);
    draw_uniform_square(draw_list, square_vec3, square_vec4, inside_border_color);
}

static void arrow_selected(ImDrawList& draw_list, const ImVec2 vec_triangle_up, const ImVec2 vec_triangle_down_left, const ImVec2 vec_triangle_down_right, ImVec2 square_vec1, ImVec2 square_vec2, ImColor selected_color, bool cond)
{
    if (cond)
    {
        // const ImVec2 arrow_selected_height = {4.f, 0.f};
        draw_triangle(draw_list, vec_triangle_up, vec_triangle_down_left, vec_triangle_down_right, selected_color);
        draw_list.AddRect(square_vec1, square_vec2, selected_color, 1.0f, ImDrawFlags_Closed);
    }
}

} // namespace internal