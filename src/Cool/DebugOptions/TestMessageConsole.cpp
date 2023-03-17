#if DEBUG

#include "TestMessageConsole.h"
#include <Cool/ImGui/ImGuiExtras.h>

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

void TestMessageConsole::imgui(Cool::MessageConsole& message_console)
{
    ImGui::NewLine();
    for (auto& message : _messages)
    {
        imgui(message, message_console);
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();
    }
    if (ImGui::Button("Add a new message ID"))
    {
        _messages.push_back({.message = std::to_string(_next_message_number++)});
        _messages.back().send_to(message_console);
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
        message_console.send(*_scoped_message_id, Cool::Message{.category = "Scoped", .message = "This is a scoped message", .severity = Cool::MessageSeverity::Error});
    }
    if (ImGui::Button("Destroy the scoped message id"))
    {
        _scoped_message_id.reset();
    }
    ImGui::Separator();
    ImGui::NewLine();
    if (ImGui::Button("Send Info to the Debug console"))
    {
        Cool::Log::Debug::info("Test", "Hello World");
    }
    if (ImGui::Button("Send Warning to the Debug console"))
    {
        Cool::Log::Debug::warning("Test", "Hello World");
    }
    if (ImGui::Button("Send Error to the Debug console (without breakpoint)"))
    {
        Cool::Log::Debug::error_without_breakpoint("Test", "Hello World");
    }
    if (ImGui::Button("Send Error to the Debug console"))
    {
        Cool::Log::Debug::error("Test", "Hello World");
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(NB: This will trigger a breakpoint)");
}

void TestMessageConsole::imgui(TestMessageConsole::Message& message, Cool::MessageConsole& message_console)
{
    ImGui::PushID(&message);
    Cool::ImGuiExtras::bring_attention_if(
        message_console.should_highlight(message.id),
        [&] {
            if (ImGui::Button("Remove"))
            {
                message_console.remove(message.id);
            }
        }
    );
    if (ImGui::Button("Send"))
    {
        message.send_to(message_console);
    }
    ImGui::InputText("Category", &message.category);
    ImGui::InputText("Message", &message.message);
    ImGui::Combo("Severity", (int*)&message.severity, "Info\0Warning\0Error\0\0");

    ImGui::PopID();
}

} // namespace Cool

#endif