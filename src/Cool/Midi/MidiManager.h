#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include "MidiChannel.h"
#include "RtMidiW/RtMidiW.hpp"

namespace Cool {

class MidiManager {
public:
    friend auto midi_manager() -> MidiManager&;

    /// Returns a number between 0 and 1 representing the current state of the controller.
    /// /!\ Note that if a knob hasn't been used yet we won't know it's current state and return 0 instead.
    [[nodiscard]] auto get_value(MidiChannel const&) const -> float;

    /// You can use this if you want to emulate a midi keyboard.
    /// `value` must be between 0 and 1.
    void set_value(MidiChannel const&, float value);

    void check_for_devices();

    /// Sets a callback that is called whenever the value of a Midi Cc changes.
    void set_additional_midi_callback(std::function<void()> callback) { _extra_midi_callback = std::move(callback); }

    void imgui_visualize_channels();
    void imgui_controllers_dropdown();
    void imgui_emulate_midi_keyboard();

private:
    MidiManager(); // Use `mini_manager()` to get the unique instance of MidiManager.

    [[nodiscard]] auto max_index() const -> int;

    void open_port(unsigned int index);
    void close_port();

    static void midi_callback(double deltatime, std::vector<unsigned char>* message, void* userData);
    static void midi_error_callback(RtMidiError::Type type, const std::string& errorText, void* userData);

private:
    RtMidiIn                       _midi_input{};
    std::string                    _port_name{};
    std::optional<unsigned int>    _port_index{};
    std::unordered_map<int, float> mIndexToValue;
    std::function<void()>          _extra_midi_callback{[] {
    }};
};

inline auto midi_manager() -> MidiManager&
{
    static auto instance = MidiManager{};
    return instance;
}

} // namespace Cool