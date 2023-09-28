#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include "MidiChannel.h"
#include "RtMidiW/RtMidiW.hpp"

namespace Cool {

class MidiManager {
public:
    /// Returns a number between 0 and 1 representing the current state of the controller.
    /// /!\ Note that if a knob hasn't been used yet we won't know it's current state and will return 0 instead.
    [[nodiscard]] auto get_value(MidiChannel const&) const -> float;

    /// You can use this if you want to emulate a midi keyboard.
    /// `value` should be between 0 and 1, since this is what people expect to receive back from `get_value()`.
    void set_value(MidiChannel const&, float value);

    /// Sets a callback that is called after the value of a Midi Channel changes.
    void set_additional_midi_callback(std::function<void()> callback) { _extra_midi_callback = std::move(callback); }

    void imgui_visualize_channels();
    void imgui_controllers_dropdown();
    void imgui_emulate_midi_keyboard();

private:
    friend auto midi_manager() -> MidiManager&;
    MidiManager(); // Use `mini_manager()` to get the unique instance of MidiManager.

    friend class AppManager;
    void check_for_devices();

    void open_port(unsigned int index);
    void close_port();

    [[nodiscard]] auto max_index() const -> int;

    static void midi_callback(double delta_time, std::vector<unsigned char>* message, void* user_data);
    static void midi_error_callback(RtMidiError::Type type, std::string const& error_text, void* user_data);

private:
    RtMidiIn                       _midi{};
    std::string                    _port_name{};
    std::optional<unsigned int>    _port_index{};
    std::unordered_map<int, float> _value_from_index{};
    std::function<void()>          _extra_midi_callback{[] {
    }};
};

inline auto midi_manager() -> MidiManager&
{
    static auto instance = MidiManager{};
    return instance;
}

} // namespace Cool