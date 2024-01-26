#include "MidiManager.h"
#include <imgui.h>
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Log/ToUser.h"

namespace Cool {

MidiManager::MidiManager()
    : _config_window{
        Cool::icon_fmt("MIDI", ICOMOON_EQUALIZER2),
        Cool::ImGuiWindowConfig{
            .is_modal   = false,
            .start_open = false,
        }
    }
{
    try
    {
        _midi.emplace();
        _midi->setErrorCallback(&midi_error_callback);
        _midi->setCallback(&midi_callback, this);
        _midi->ignoreTypes(true, true, true); // ignore sysex, timing, and active sensing messages.
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("MIDI Error", e.what());
    }
}

auto MidiManager::get_value(MidiChannel const& channel) const -> float
{
    auto const it = _value_from_index.find(channel.index);
    if (it == _value_from_index.end())
        return 0.f;
    return it->second;
}

void MidiManager::set_value(MidiChannel const& channel, float value)
{
    _value_from_index[channel.index] = value;
}

void MidiManager::check_for_devices()
{
    if (!_midi.has_value())
        return;

    try
    {
        auto const port_count = _midi->getPortCount();

        if (_port_index.has_value()
            && *_port_index < port_count
            && _midi->getPortName(*_port_index) == _port_name)
        {
            return; // The port we have opened still exists and hasn't been moved because of the creation of new ports.
        }

        if (port_count > 0)
            open_port(0);
        else
            close_port();
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("MIDI Error", e.what());
    }
}

void MidiManager::midi_callback(double /* delta_time */, std::vector<unsigned char>* message, void* user_data)
{
    if (message->size() < 3)
        return;
    // TODO(OSC) don't we need a lock / Mutex ?
    // TODO(OSC) rerender when new midi value arrives, if we depend on that value (like what we did for OSC)
    auto& This                            = *static_cast<MidiManager*>(user_data);
    This._value_from_index[(*message)[1]] = static_cast<float>((*message)[2]) / 127.f;
    This._extra_midi_callback();
}

void MidiManager::midi_error_callback(RtMidiError::Type /* type */, const std::string& error_text, void* /* user_data */)
{
    Cool::Log::ToUser::warning("MIDI", error_text);
}

auto MidiManager::max_index() const -> int
{
    auto const it = std::max_element(_value_from_index.begin(), _value_from_index.end(), [](auto const& p1, auto const& p2) {
        return p1.first < p2.first; // Compare the indices
    });

    if (it == _value_from_index.end())
        return 0;
    return it->first;
}

void MidiManager::open_port(unsigned int index)
{
    if (!_midi.has_value())
        return;

    try
    {
        _midi->closePort();
        _midi->openPort(index);
        _port_name  = _midi->getPortName(index);
        _port_index = index;
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("MIDI Error", e.what());
    }
}

void MidiManager::close_port()
{
    if (!_midi.has_value())
        return;

    try
    {
        _midi->closePort();
        _port_name.clear();
        _port_index.reset();
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("MIDI Error", e.what());
    }
}

void MidiManager::imgui_window()
{
    _config_window.show([&]() {
        imgui_visualize_channels();
        imgui_controllers_dropdown();
    });
}

void MidiManager::imgui_visualize_channels()
{
    auto values = std::vector<float>(static_cast<size_t>(max_index() + 1));
    for (size_t i = 0; i < values.size(); ++i)
        values[i] = get_value({static_cast<int>(i)});

    ImGui::PushFont(Font::italic());
    ImGui::TextUnformatted("Use your knob / slider / button and the value will reflect in the histogram below. You can then hover it to see the index.");
    ImGui::PopFont();

    ImGui::PlotHistogram("Channels", values.data(), static_cast<int>(values.size()), 0, nullptr, 0.f, 1.f, ImVec2(0, 80.0f));
}

void MidiManager::imgui_controllers_dropdown()
{
    if (!_midi.has_value())
        return;

    try
    {
        if (ImGui::BeginCombo("Controller", _port_name.c_str()))
        {
            for (unsigned int i = 0; i < _midi->getPortCount(); ++i)
            {
                if (ImGui::Selectable(_midi->getPortName(i).c_str()))
                    open_port(i);
            }
            ImGui::EndCombo();
        }
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("MIDI Error", e.what());
    }
}

void MidiManager::imgui_emulate_midi_keyboard()
{
    static auto values = std::vector<float>(31);
    for (int i = 0; i < static_cast<int>(values.size()); ++i)
    {
        ImGui::PushID(i);
        if (ImGui::VSliderFloat("", ImVec2(30, 160), &values[static_cast<size_t>(i)], 0.f, 1.f, ""))
        {
            std::vector<unsigned char> message{0, static_cast<unsigned char>(i), static_cast<unsigned char>(values[static_cast<size_t>(i)] * 127.f)};
            midi_callback(0.f, &message, this);
        }
        ImGui::SetItemTooltip("%i", i);
        if (i != static_cast<int>(values.size()) - 1)
            ImGui::SameLine();
        ImGui::PopID();
    }
    if (ImGui::Button("Add channel"))
        values.push_back(0.f);
    if (ImGui::Button("Remove channel") && !values.empty())
        values.pop_back();
}

} // namespace Cool