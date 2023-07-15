#include "MidiManager.h"

namespace Cool {

void MidiManager::connect()
{
    // TODO(Midi) connect to midi device
    mPort = 0;
    mMidiIn = new RtMidiIn();
    mNumPorts = mMidiIn->getPortCount();
    mMidiIn->getCurrentApi();
    if (mNumPorts > 0) {
		mName = mMidiIn->getPortName(mPort);
		mMidiIn->openPort(mPort);
		std::cout << "Opening MIDI port 0" << std::endl;
		//mMidiIn.midiSignal.connect(std::bind(&Midi2WebsocketApp::midiListener, this, std::placeholders::_1));
	    //mMidiIn->setCallback(&MidiInCallback, this);
	    //mMidiIn->ignoreTypes(false, false, false);
	}
	else {
		std::cout << "No MIDI Ports found!" << std::endl;
	}
	

}
void MidiManager::disconnect()
{
    mMidiIn->closePort();
	mMidiIn->cancelCallback();
}

void MidiManager::imgui()
{
    
   
    if (ImGui::Button("Connect"))
    {
        connect();
    }
   
}

}