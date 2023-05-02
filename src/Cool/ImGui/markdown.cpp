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

static void format_emphasis(ImGui::MarkdownFormatInfo const& info, bool is_beginning)
{
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
        auto const fmt = ImGui::MarkdownHeadingFormat{info.config->headingFormats[ImGui::MarkdownConfig::NUMHEADINGS - 1]};
        if (is_beginning)
        {
            if (fmt.font)
                ImGui::PushFont(fmt.font);
        }
        else
        {
            if (fmt.font)
                ImGui::PopFont();
        }
    }
}

static void format_heading(ImGui::MarkdownFormatInfo const& info, bool is_beginning)
{
    ImGui::MarkdownHeadingFormat fmt = info.level > ImGui::MarkdownConfig::NUMHEADINGS
                                           ? info.config->headingFormats[ImGui::MarkdownConfig::NUMHEADINGS - 1]
                                           : info.config->headingFormats[info.level - 1]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    if (is_beginning)
    {
        if (fmt.font)
            ImGui::PushFont(fmt.font);
        ImGui::NewLine();
    }
    else
    {
        if (fmt.separator)
            ImGui::Separator();
        ImGui::NewLine();
        if (fmt.font)
            ImGui::PopFont();
    }
}

static void format_link(ImGui::MarkdownFormatInfo const& info, bool is_beginning)
{
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
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
}

static void format_callback(ImGui::MarkdownFormatInfo const& info, bool is_beginning)
{
    switch (info.type)
    {
    case ImGui::MarkdownFormatType::NORMAL_TEXT:
        break;
    case ImGui::MarkdownFormatType::EMPHASIS:
    {
        format_emphasis(info, is_beginning);
        break;
    }
    case ImGui::MarkdownFormatType::HEADING:
    {
        format_heading(info, is_beginning);
        break;
    }
    case ImGui::MarkdownFormatType::UNORDERED_LIST:
        break;
    case ImGui::MarkdownFormatType::LINK:
        format_link(info, is_beginning);
        break;
    }
}

} // namespace Cool::ImGuiExtras