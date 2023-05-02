#include "markdown.h"
#include <imgui_markdown/imgui_markdown.h>
#include <open_link/open_link.hpp>
#include "imgui.h"

namespace Cool::ImGuiExtras {

static void link_clicked_callback(ImGui::MarkdownLinkCallbackData data)
{
    if (data.isImage)
        return;
    open_link(std::string{data.link, static_cast<size_t>(data.linkLength)}.c_str()); // `link` is not a zero-terminated string, so we must construct a string from the pointer and the length.
}

static void format_callback(ImGui::MarkdownFormatInfo const& info, bool is_beginning);

void markdown(std::string_view markdown_text)
{
    static auto const config = ImGui::MarkdownConfig{
        .linkCallback = &link_clicked_callback,
        // .tooltipCallback   = NULL;
        .formatCallback = format_callback,
    };
    // mdConfig.imageCallback     = ImageCallback;
    // mdConfig.headingFormats[0] = {H1, true};
    // mdConfig.headingFormats[1] = {H2, true};
    // mdConfig.headingFormats[2] = {H3, false};
    // mdConfig.userData          = NULL;
    ImGui::Markdown(markdown_text.data(), markdown_text.length(), config);
}

static void format_callback(ImGui::MarkdownFormatInfo const& info, bool is_beginning)
{
    switch (info.type)
    {
    case ImGui::MarkdownFormatType::NORMAL_TEXT:
        break;
    case ImGui::MarkdownFormatType::EMPHASIS:
    {
        ImGui::MarkdownHeadingFormat fmt;
        // default styling for emphasis uses last headingFormats - for your own styling
        // implement EMPHASIS in your formatCallback
        if (info.level == 1)
        {
            // normal emphasis
            if (is_beginning)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            }
            else
            {
                ImGui::PopStyleColor();
            }
        }
        else
        {
            // strong emphasis
            fmt = info.config->headingFormats[ImGui::MarkdownConfig::NUMHEADINGS - 1];
            if (is_beginning)
            {
                if (fmt.font)
                {
                    ImGui::PushFont(fmt.font);
                }
            }
            else
            {
                if (fmt.font)
                {
                    ImGui::PopFont();
                }
            }
        }
        break;
    }
    case ImGui::MarkdownFormatType::HEADING:
    {
        ImGui::MarkdownHeadingFormat fmt;
        if (info.level > ImGui::MarkdownConfig::NUMHEADINGS)
        {
            fmt = info.config->headingFormats[ImGui::MarkdownConfig::NUMHEADINGS - 1];
        }
        else
        {
            fmt = info.config->headingFormats[info.level - 1];
        }
        if (is_beginning)
        {
            if (fmt.font)
            {
                ImGui::PushFont(fmt.font);
            }
            ImGui::NewLine();
        }
        else
        {
            if (fmt.separator)
            {
                ImGui::Separator();
                ImGui::NewLine();
            }
            else
            {
                ImGui::NewLine();
            }
            if (fmt.font)
            {
                ImGui::PopFont();
            }
        }
        break;
    }
    case ImGui::MarkdownFormatType::UNORDERED_LIST:
        break;
    case ImGui::MarkdownFormatType::LINK:
        if (is_beginning)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
        }
        else
        {
            ImGui::PopStyleColor();
            if (info.itemHovered)
            {
                ImGui::UnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
            }
            else
            {
                ImGui::UnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
            }
        }
        break;
    }

    switch (info.type)
    {
    // example: change the colour of heading level 2
    case ImGui::MarkdownFormatType::HEADING:
    {
        if (info.level == 2)
        {
            if (is_beginning)
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

} // namespace Cool::ImGuiExtras