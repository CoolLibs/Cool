#include "TestMessageConsole.h"

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

void TestMessageConsole::imgui_window(Cool::MessageConsole& message_console, bool* is_open)
{
    if (_should_bring_window_to_front)
    {
        _should_bring_window_to_front = false;
        ImGui::SetNextWindowToFront();
    }
    ImGui::Begin("Test Message Console", is_open);
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
    ImGui::End();
}

void TestMessageConsole::imgui(TestMessageConsole::Message& message, Cool::MessageConsole& message_console)
{
    ImGui::PushID(&message);
    if (ImGui::Button("Clear"))
    {
        message_console.clear(message.id);
    }
    if (message_console.should_highlight(message.id))
    {
        ImGui::SameLine();
        ImGui::Text("<-- CLICK HERE");
        ImGui::SetScrollHereY(0.5f);
        _should_bring_window_to_front = true;
    }
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
