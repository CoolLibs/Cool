#include "MidiManager.h"
#include <RtMidiWrapper/rtmidi/RtMidi.h>
#include <imgui.h>
#include "Cool/Log/ToUser.h"
#include "Cool/Midi/MidiManager.h"

namespace Cool {

MidiManager::MidiManager()
{
    _midi_input.setErrorCallback(&midi_error_callback);
    _midi_input.setCallback(&midi_callback, this);
    _midi_input.ignoreTypes(true, true, true); // ignore sysex, timing, and active sensing messages.
}

auto MidiManager::get_value(MidiChannel const& channel) const -> float
{
    auto const it = mIndexToValue.find(channel.index);
    if (it == mIndexToValue.end())
        return 0.f;
    return it->second;
}

void MidiManager::set_value(MidiChannel const& channel, float value)
{
    mIndexToValue[channel.index] = value;
}

void MidiManager::check_for_devices()
{
    auto const port_count = _midi_input.getPortCount();

    if (_port_index.has_value()
        && *_port_index < port_count
        && _midi_input.getPortName(*_port_index) == _port_name)
    {
        return; // The port we have opened still exists and hasn't been moved because of the creation of new ports.
    }

    if (port_count > 0)
        open_port(0);
    else
        close_port();
}

void MidiManager::midi_callback(double /* delta_time */, std::vector<unsigned char>* message, void* user_data)
{
    if (message->size() < 3)
        return;

    auto* This                          = static_cast<MidiManager*>(user_data);
    This->mIndexToValue[message->at(1)] = static_cast<float>(message->at(2)) / 127.f;
    This->_extra_midi_callback();
}

void MidiManager::midi_error_callback(RtMidiError::Type /* type */, const std::string& errorText, void* /* userData */)
{
    Cool::Log::ToUser::warning("MIDI", errorText);
}

auto MidiManager::max_index() const -> int
{
    auto const it = std::max_element(mIndexToValue.begin(), mIndexToValue.end(), [](auto const& p1, auto const& p2) {
        return p1.first < p2.first; // Compare the indices
    });

    if (it == mIndexToValue.end())
        return 0;
    return it->first;
}

void MidiManager::open_port(unsigned int index)
{
    _midi_input.closePort();
    _midi_input.openPort(index);
    _port_name  = _midi_input.getPortName(index);
    _port_index = index;
}

void MidiManager::close_port()
{
    _midi_input.closePort();
    _port_name.clear();
    _port_index.reset();
}

void MidiManager::imgui_visualize_channels()
{
    auto values = std::vector<float>(max_index() + 1);
    for (int i = 0; i < static_cast<int>(values.size()); ++i)
        values[i] = get_value({i});

    ImGui::PlotHistogram("Channels", values.data(), static_cast<int>(values.size()), 0, nullptr, 0.f, 1.f, ImVec2(0, 80.0f));
}

void MidiManager::imgui_controllers_dropdown()
{
    if (ImGui::BeginCombo("Controller", _port_name.c_str()))
    {
        for (unsigned int i = 0; i < _midi_input.getPortCount(); ++i)
        {
            if (ImGui::Selectable(_midi_input.getPortName(i).c_str()))
                open_port(i);
        }
        ImGui::EndCombo();
    }
}

void MidiManager::imgui_emulate_midi_keyboard()
{
    static auto values = std::vector<float>(31);
    for (int i = 0; i < static_cast<int>(values.size()); ++i)
    {
        ImGui::PushID(i);
        if (ImGui::VSliderFloat("", ImVec2(30, 160), &values[i], 0.f, 1.f, ""))
        {
            std::vector<unsigned char> message{0, static_cast<unsigned char>(i), static_cast<unsigned char>(values[i] * 127)};
            midi_callback(0.f, &message, this);
        }
        ImGui::SetItemTooltip("%i", i);
        if (i != static_cast<int>(values.size()) - 1)
            ImGui::SameLine();
        ImGui::PopID();
    }
    for (unsigned int i = 0; i < _midi_input.getPortCount(); ++i)
    {
        ImGui::TextUnformatted(_midi_input.getPortName(i).c_str());
    }
    if (ImGui::Button("Add channel"))
        values.push_back(0.f);
    if (ImGui::Button("Remove channel"))
        values.pop_back();
}

} // namespace Cool