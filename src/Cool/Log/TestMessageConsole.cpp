#include "TestMessageConsole.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "message_console.hpp"

namespace Cool {

TestMessageConsole::TestMessageConsole()
{
    _messages.push_back({}); // We can't use an initializer list to fill a container of move-only types, so we have to push_back manually
    _messages.push_back({
        "Test 2",
        "Hello! 2\nmulti\nline",
        Cool::MessageSeverity::Warning,
    });
}

void TestMessageConsole::imgui()
{
    ImGui::NewLine();
    for (auto& message : _messages)
    {
        imgui(message);
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();
    }
    if (ImGui::Button("Add a new message ID"))
    {
        _messages.push_back({.message = std::to_string(_next_message_number++)});
        _messages.back().send();
    }
    if (ImGui::Button("Remove last message ID"))
    {
        if (!_messages.empty())
        {
            _messages.pop_back();
        }
    }
    ImGui::Separator();
    ImGui::NewLine();
    if (ImGui::Button("Send a scoped message"))
    {
        _scoped_message_id.emplace();
        message_console().send(*_scoped_message_id, Cool::Message{.category = "Scoped", .message = "This is a scoped message", .severity = Cool::MessageSeverity::Error});
    }
    if (ImGui::Button("Destroy the scoped message id"))
    {
        _scoped_message_id.reset();
    }
}

void TestMessageConsole::imgui(TestMessageConsole::Message& message)
{
    ImGui::PushID(&message);
    Cool::ImGuiExtras::bring_attention_if(
        message_console().should_highlight(message.id),
        [&] {
            if (ImGui::Button("Remove"))
            {
                message_console().remove(message.id);
            }
        }
    );
    if (ImGui::Button("Send"))
    {
        message.send();
    }
    ImGui::InputText("Category", &message.category);
    ImGui::InputText("Message", &message.message);
    ImGui::Combo("Severity", (int*)&message.severity, "Info\0Warning\0Error\0\0");

    ImGui::PopID();
}

} // namespace Cool
