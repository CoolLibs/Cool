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
    auto get_value(MidiCc const&) -> float;
    void connect();
    void disconnect();
    void imgui();
    /// Sets a callback that is called whenever the value of a Midi Cc changes.
    void set_additional_midi_callback(std::function<void()> callback) { _extra_midi_callback = std::move(callback); }

    void imgui_emulate_midi_keyboard();

private:
    RtMidiIn*    mMidiIn;
    unsigned int mNumPorts;
    unsigned int mPort;
    std::string  mName;
    static void  midiCallback(double deltatime, std::vector<unsigned char>* message, void* userData);
    RtMidi::Api  chooseMidiApi();
    // imgui
    char buf[64];
    // TODO(Midi) map index vers float
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