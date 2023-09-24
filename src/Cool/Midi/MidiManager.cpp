#include "MidiManager.h"
#include <imgui.h>
#include "Cool/Midi/RtMidi.h"
#include "RtMidi.h"

namespace Cool {

MidiManager::MidiManager()
    : _midi_in_instance{RtMidi::Api::UNSPECIFIED}
{
    _midi_in_instance.setCallback(&midi_callback, this);
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

void MidiManager::midi_callback(double /* delta_time */, std::vector<unsigned char>* message, void* user_data)
{
    if (message->size() < 3)
        return;

    auto* This                          = static_cast<MidiManager*>(user_data);
    This->mIndexToValue[message->at(1)] = static_cast<float>(message->at(2)) / 127.f;
    This->_extra_midi_callback();
}

void MidiManager::connect()
{
    // mNumPorts = _midi_in_instance.getPortCount();

    // for (size_t i = 0; i < mNumPorts; ++i)
    // {
    //     std::cout << i << ": " << _midi_in_instance.getPortName(i).c_str() << std::endl;
    //     std::string name(_midi_in_instance.getPortName(i).c_str()); // strip null chars introduced by rtmidi
    // }

    // // _midi_in_instance.getCurrentApi();
    // if (mNumPorts > 0)
    // {
    //     mName = _midi_in_instance.getPortName(mPort);
    //     _midi_in_instance.openPort(mPort);
    //     _midi_in_instance.ignoreTypes(true, true, true); // ignore sysex, timing, and active sensing messages.
    // }
}

void MidiManager::disconnect()
{
    _midi_in_instance.closePort();
    _midi_in_instance.cancelCallback();
}

void MidiManager::imgui()
{
    ImGui::Text("%f", get_value(MidiChannel{1}));
    //    ImGui::Begin(buf);
    {
        ImGui::PushItemWidth(80);

        ImGui::TextColored(ImColor(155, 50, 255), "%s-", "bob");

        ImGui::PopItemWidth();
    }
    // ImGui::End();
    // if (ImGui::Button("Connect"))
    // {
    //     connect();
    // }
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