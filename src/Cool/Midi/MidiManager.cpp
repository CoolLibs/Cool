#include "MidiManager.h"
#include "RtMidi.h"

namespace Cool {

RtMidi::Api MidiManager::chooseMidiApi()
{
  /*std::vector< RtMidi::Api > apis;
  RtMidi::getCompiledApi( apis );

  if (apis.size() <= 1)
      return RtMidi::Api::UNSPECIFIED;

  std::cout << "\nAPIs\n  API #0: unspecified / default\n";
  for (size_t n = 0; n < apis.size(); n++)
      std::cout << "  API #" << apis[n] << ": " << RtMidi::getApiDisplayName(apis[n]) << "\n";

  std::cout << "\nChoose an API number: ";
  unsigned int i;
  std::cin >> i;

  std::string dummy;
  std::getline( std::cin, dummy ); */ // used to clear out stdin

  return RtMidi::Api::WINDOWS_MM;// static_cast< RtMidi::Api >( 0 ); //i );
}
void midiCallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
  unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
}
void MidiManager::connect()
{
    // TODO(Midi) connect to midi device
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
    
   
    if (ImGui::Button("Connect"))
    {
        connect();
    }
   
}

}