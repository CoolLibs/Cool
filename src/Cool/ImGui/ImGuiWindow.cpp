#include "ImGuiWindow.h"

namespace Cool {

void ImGuiWindow::show(std::function<void()> widgets)
{
    if (_is_open)
    {
        ImGui::Begin(_title, &_is_open);
        widgets();
        ImGui::End();
    }
}

void ImGuiWindow::open_close_checkbox()
{
    bool should_open = _is_open;
    if (ImGui::Checkbox(_title, &should_open))
    {
        if (should_open)
        {
            open();
        }
        else
        {
            close();
        }
    }
}

} // namespace Cool
