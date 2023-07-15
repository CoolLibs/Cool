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
    //void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData );
    RtMidi::Api chooseMidiApi();

// TODO(Midi) map index vers float
    };

inline auto midi_manager() -> MidiManager&
{
    std::cout << "midi_manager" << std::endl;
    static auto instance = MidiManager{};
    instance.connect();
    return instance;
}

}