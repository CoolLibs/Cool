#pragma once
#define __RTMIDI_DEBUG__
#include <iostream>
#include <string>
#include <vector>
#include "MidiCc.h"
#include "RtMidi.h"
// #include "MidiHeaders.h"

namespace Cool {

class MidiManager {
public:
    friend auto midi_manager() -> MidiManager&;

    /// Returns a number between 0 and 1 representing the current state of the controller.
    /// /!\ Note that if a knob hasn't been used yet we won't know it's current state and return 0 instead.
    [[nodiscard]] auto get_value(MidiCc const&) const -> float;

    /// You can use this if you want to emulate a midi keyboard.
    /// `value` must be between 0 and 1.
    void set_value(MidiCc const&, float value);

    void connect();
    void disconnect();
    void imgui();
    /// Sets a callback that is called whenever the value of a Midi Cc changes.
    void set_additional_midi_callback(std::function<void()> callback) { _extra_midi_callback = std::move(callback); }

    void imgui_emulate_midi_keyboard();
    void imgui_visualize_channels();

private:
    MidiManager() = default; // Use `mini_manager()` to get the unique instance of MidiManager.

    [[nodiscard]] auto max_index() const -> int;

private:
    RtMidiIn*    mMidiIn;
    unsigned int mNumPorts;
    unsigned int mPort;
    std::string  mName;
    static void  midiCallback(double deltatime, std::vector<unsigned char>* message, void* userData);
    RtMidi::Api  chooseMidiApi();
    // imgui
    char                           buf[64];
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