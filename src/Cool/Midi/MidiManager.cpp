#include "MidiManager.h"
#include <imgui.h>
#include "RtMidi.h"

namespace Cool {

RtMidi::Api MidiManager::chooseMidiApi()
{
  return  static_cast< RtMidi::Api >( 0 ); //i );
  //ok return RtMidi::Api::WINDOWS_MM;
}
auto MidiManager::get_value(MidiCc const& midiCc) -> float
{
    return mIndexToValue[midiCc.index];
}
void  MidiManager::midiCallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
    MidiManager* midiManager = static_cast<MidiManager*>(userData);
    unsigned int nBytes = message->size();
    if ( nBytes > 2 ) midiManager->mIndexToValue[message->at(1)] = message->at(2) / 127.f;
}
void MidiManager::connect()
{

    mPort = 0;
    
    /*RtMidiIn *midiin = new RtMidiIn(chooseMidiApi());
  // Check available ports.
  unsigned int nPorts = midiin->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
   
  }
  midiin->openPort( 0 );
  // Set our callback function.  This should be done immediately after
  // opening the port to avoid having incoming messages written to the
  // queue.
  //midiin->setCallback( &mycallback );
  // ignore sysex, timing, or active sensing messages.
  midiin->ignoreTypes( true, true, true );
  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
  char input;
  std::cin.get(input);*/
    mMidiIn = new RtMidiIn(chooseMidiApi());
    mMidiIn->setCallback(&midiCallback, this);
    mNumPorts = mMidiIn->getPortCount();
    auto const sd =   mMidiIn->getCurrentApi();


    std::cout << "MidiIn: " << mNumPorts << " available." << std::endl;
    for (size_t i = 0; i < mNumPorts; ++i){
        std::cout << i << ": " << mMidiIn->getPortName(i).c_str() << std::endl;
        std::string name( mMidiIn->getPortName( i ).c_str() ); // strip null chars introduced by rtmidi
        sprintf(buf, " %s##s%d", mMidiIn->getPortName( i ).c_str(), i);
    }

    // mMidiIn->getCurrentApi();
    if (mNumPorts > 0) {
		mName = mMidiIn->getPortName(mPort);
		mMidiIn->openPort(mPort);
		std::cout << "Opening MIDI port 0" << std::endl;
		//mMidiIn.midiSignal.connect(std::bind(&Midi2WebsocketApp::midiListener, this, std::placeholders::_1));
	    //mMidiIn->setCallback(&MidiInCallback, this);
	    mMidiIn->ignoreTypes(true, true, true);
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
    ImGui::Text("%f", get_value(MidiCc{1}));
//    ImGui::Begin(buf);
			{
				ImGui::PushItemWidth(80);
				
				ImGui::TextColored(ImColor(155, 50, 255), "%s-", buf);
				
				
				ImGui::PopItemWidth();
			}
			// ImGui::End();
    // if (ImGui::Button("Connect"))
    // {
    //     connect();
    // }
   
}

void MidiManager::imgui_emulate_midi_keyboard()
{
    static auto values = std::vector<float>(6);
    for (size_t i = 0; i < values.size(); ++i)
    {
        if(ImGui::SliderFloat(fmt::format("Index {}", i).c_str(), &values[i], 0.f, 1.f))
        {
            std::vector<unsigned char> message{0, static_cast<unsigned char>(i), static_cast<unsigned char>(values[i] * 127)};
            midiCallback(0.f, &message, this);
        }
    }
}

}