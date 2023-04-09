#include "ImGuiWindow.h"
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool {

void ImGuiWindow::show(std::function<void()> widgets)
{
    if (_is_open)
    {
        ImGui::Begin(_title.c_str(), &_is_open);
        widgets();
        ImGui::End();
    }
}

void ImGuiWindow::open_close_checkbox()
{
    bool should_open = _is_open;
    if (ImGuiExtras::toggle(_title.c_str(), &should_open))
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
