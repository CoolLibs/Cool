#include "ImNodesHelpers.h"
#include "imnodes/imnodes.h"

namespace Cool {

static auto as_ImU32(Cool::Color const& color) -> ImU32
{
    auto const col_srgb = color.as_sRGB();
    return ImGui::GetColorU32(ImVec4{col_srgb.x, col_srgb.y, col_srgb.z, 1.f});
}

ScopedTitleBarColor::ScopedTitleBarColor(Cool::Color const& color)
{
    auto const category_color        = as_ImU32(color);
    auto const bright_category_color = as_ImU32(color.brighter());

    ImNodes::PushColorStyle(ImNodesCol_TitleBar, category_color);
    ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, bright_category_color);
    ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, bright_category_color);
}

ScopedTitleBarColor::~ScopedTitleBarColor()
{
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

} // namespace Cool