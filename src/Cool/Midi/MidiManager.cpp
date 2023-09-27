#include "MidiManager.h"
#include <RtMidiWrapper/rtmidi/RtMidi.h>
#include <imgui.h>
#include "Cool/Log/ToUser.h"
#include "Cool/Midi/MidiManager.h"

namespace Cool {

MidiManager::MidiManager()
{
    _midi_in_checker.setErrorCallback(&midi_error_callback);
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
    { // Remove all ports that have closed
        auto to_remove = std::vector<std::string>{};
        for (auto& pair : _midi_inputs)
        {
            if (!pair.second.isPortOpen())
                to_remove.push_back(pair.first);
        }
        for (auto const& port_name : to_remove)
            _midi_inputs.erase(port_name);
    }

    // Add all new ports
    for (unsigned int i = 0; i < _midi_in_checker.getPortCount(); ++i)
    {
        auto const name = _midi_in_checker.getPortName(i);
        auto const it   = _midi_inputs.find(name);
        if (it != _midi_inputs.end())
            continue;

        auto const key_value = _midi_inputs.insert(std::make_pair(name, RtMidiIn{})).first;
        key_value->second.setErrorCallback(&midi_error_callback);
        key_value->second.setCallback(&midi_callback, this);
        key_value->second.ignoreTypes(true, true, true); // ignore sysex, timing, and active sensing messages.
        key_value->second.openPort(0);
    }
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

// void MidiManager::connect()
// {
//     // mNumPorts = _midi_in_checker.getPortCount();

//     // for (size_t i = 0; i < mNumPorts; ++i)
//     // {
//     //     std::cout << i << ": " << _midi_in_checker.getPortName(i).c_str() << std::endl;
//     //     std::string name(_midi_in_checker.getPortName(i).c_str()); // strip null chars introduced by rtmidi
//     // }

//     // // _midi_in_checker.getCurrentApi();
//     // if (mNumPorts > 0)
//     // {
//     //     mName = _midi_in_checker.getPortName(mPort);
//     //     _midi_in_checker.openPort(mPort);
//     //     _midi_in_checker.ignoreTypes(true, true, true); // ignore sysex, timing, and active sensing messages.
//     // }
// }

// void MidiManager::disconnect()
// {
//     _midi_in_checker.closePort();
//     _midi_in_checker.cancelCallback();
// }

auto MidiManager::max_index() const -> int
{
    auto const it = std::max_element(mIndexToValue.begin(), mIndexToValue.end(), [](auto const& p1, auto const& p2) {
        return p1.first < p2.first; // Compare the indices
    });

    if (it == mIndexToValue.end())
        return 0;
    return it->first;
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
    for (unsigned int i = 0; i < _midi_in_checker.getPortCount(); ++i)
    {
        ImGui::TextUnformatted(_midi_in_checker.getPortName(i).c_str());
    }
    if (!_midi_in_checker.isPortOpen() && _midi_in_checker.getPortCount() > 0)
        _midi_in_checker.openPort(0);
    if (_midi_in_checker.getPortCount() == 0)
        _midi_in_checker.closePort();
}

void MidiManager::imgui_visualize_channels()
{
    // ImGui::Text(buf);
    auto values = std::vector<float>(max_index() + 1);
    for (int i = 0; i < static_cast<int>(values.size()); ++i)
        values[i] = get_value({i});

    ImGui::PlotHistogram("Channels", values.data(), static_cast<int>(values.size()), 0, nullptr, 0.f, 1.f, ImVec2(0, 80.0f));
}

} // namespace Cool