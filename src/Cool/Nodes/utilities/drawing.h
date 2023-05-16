#pragma once
#include <imgui.h>

namespace ax {
namespace Drawing {

enum class IconType : ImU32 { Flow,
                              Circle,
                              Square,
                              Grid,
                              RoundSquare,
                              Diamond };

void DrawIcon(ImDrawList* drawList, const ImVec2& a, const ImVec2& b, IconType type, bool filled, ImU32 color, ImU32 innerColor);

}
} // namespace ax::Drawing

namespace Cool {

void show_label(const char* label, ImColor color);

}