#include "ImGuiWindow.h"
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool {

void ImGuiWindow::show(std::function<void()> widgets)
{
    if (_is_modal)
    {
        bool const is_really_open = ImGui::IsPopupOpen(_title.c_str());
        if (!is_really_open && _is_open)
            ImGui::OpenPopup(_title.c_str());
        _is_open = ImGui::IsPopupOpen(_title.c_str()); // If the popup has been closed, _is_open must reflect that.

        if (ImGui::BeginPopupModal(_title.c_str(), &_is_open, ImGuiWindowFlags_AlwaysAutoResize))
        {
            widgets();
            ImGui::EndPopup();
        }
    }
    else
    {
        if (_is_open)
        {
            if (ImGui::Begin(_title.c_str(), &_is_open))
                widgets();
            ImGui::End();
        }
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
