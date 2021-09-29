#include "ImGuiWindow.h"

namespace Cool {

void ImGuiWindow::show(std::function<void()> widgets)
{
    if (_is_open) {
        ImGui::Begin(_title, &_is_open);
        widgets();
        ImGui::End();
    }
}

} // namespace Cool
