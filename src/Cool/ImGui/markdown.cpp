#include "markdown.h"
#include <imgui_markdown/imgui_markdown.h>
#include <open_link/open_link.hpp>
#include "imgui.h"

namespace Cool::ImGuiExtras {

static void link_callback(ImGui::MarkdownLinkCallbackData data)
{
    if (data.isImage)
        return;
    auto const url = std::string{data.link, static_cast<size_t>(data.linkLength)};
    open_link(url.c_str());
}

static void ExampleMarkdownFormatCallback(const ImGui::MarkdownFormatInfo& markdownFormatInfo_, bool start_)
{
    // Call the default first so any settings can be overwritten by our implementation.
    // Alternatively could be called or not called in a switch statement on a case by case basis.
    // See defaultMarkdownFormatCallback definition for furhter examples of how to use it.
    ImGui::defaultMarkdownFormatCallback(markdownFormatInfo_, start_);

    switch (markdownFormatInfo_.type)
    {
    // example: change the colour of heading level 2
    case ImGui::MarkdownFormatType::HEADING:
    {
        if (markdownFormatInfo_.level == 2)
        {
            if (start_)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            }
            else
            {
                ImGui::PopStyleColor();
            }
        }
        break;
    }
    case ImGui::MarkdownFormatType::LINK:
    {
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
    default:
    {
        break;
    }
    }
}

void markdown(std::string_view markdown_text)
{
    static auto const config = ImGui::MarkdownConfig{
        .linkCallback = &link_callback,
        // .tooltipCallback   = NULL;
        .formatCallback = ExampleMarkdownFormatCallback,
    };
    // mdConfig.imageCallback     = ImageCallback;
    // mdConfig.headingFormats[0] = {H1, true};
    // mdConfig.headingFormats[1] = {H2, true};
    // mdConfig.headingFormats[2] = {H3, false};
    // mdConfig.userData          = NULL;
    ImGui::Markdown(markdown_text.data(), markdown_text.length(), config);
}

} // namespace Cool::ImGuiExtras