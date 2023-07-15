#pragma once
#define __RTMIDI_DEBUG__
#include "MidiCc.h"
#include <vector>
#include <string>
#include <iostream>
#include "RtMidi.h"
//#include "MidiHeaders.h"

namespace Cool {

class MidiManager{
public:
    auto get_value(MidiCc const& ) -> float ;
    void connect();
    void disconnect();
    void imgui();
private:
    RtMidiIn*       mMidiIn;
	unsigned int    mNumPorts;
	unsigned int    mPort;
	std::string     mName;
     static  void midiCallback( double deltatime, std::vector< unsigned char > *message, void *userData );
    RtMidi::Api chooseMidiApi();
// imgui
		char						buf[64];
// TODO(Midi) map index vers float
std::unordered_map<int, float> mIndexToValue;
    };

inline auto midi_manager() -> MidiManager&
{
    static auto instance = MidiManager{};
    return instance;
}

}