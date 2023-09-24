#include "MidiManager.h"
#include <imgui.h>
#include "RtMidi.h"

namespace Cool {

RtMidi::Api MidiManager::chooseMidiApi()
{
    return static_cast<RtMidi::Api>(0); // i );
    // ok return RtMidi::Api::WINDOWS_MM;
}

auto MidiManager::get_value(MidiCc const& midiCc) const -> float
{
    auto const it = mIndexToValue.find(midiCc.index);
    if (it == mIndexToValue.end())
        return 0.f;
    return it->second;
}

void MidiManager::set_value(MidiCc const& cc, float value)
{
    mIndexToValue[cc.index] = value;
}

void MidiManager::midiCallback(double /* delta_time */, std::vector<unsigned char>* message, void* user_data)
{
    MidiManager* midiManager = static_cast<MidiManager*>(user_data);
    unsigned int nBytes      = message->size();
    if (nBytes > 2)
    {
        midiManager->mIndexToValue[message->at(1)] = message->at(2) / 127.f;
        midiManager->_extra_midi_callback();
    }
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
    mNumPorts     = mMidiIn->getPortCount();
    auto const sd = mMidiIn->getCurrentApi();

    std::cout << "MidiIn: " << mNumPorts << " available." << std::endl;
    for (size_t i = 0; i < mNumPorts; ++i)
    {
        std::cout << i << ": " << mMidiIn->getPortName(i).c_str() << std::endl;
        std::string name(mMidiIn->getPortName(i).c_str()); // strip null chars introduced by rtmidi
        sprintf(buf, " %s##s%d", mMidiIn->getPortName(i).c_str(), i);
    }

    // mMidiIn->getCurrentApi();
    if (mNumPorts > 0)
    {
        mName = mMidiIn->getPortName(mPort);
        mMidiIn->openPort(mPort);
        std::cout << "Opening MIDI port 0" << std::endl;
        // mMidiIn.midiSignal.connect(std::bind(&Midi2WebsocketApp::midiListener, this, std::placeholders::_1));
        // mMidiIn->setCallback(&MidiInCallback, this);
        mMidiIn->ignoreTypes(true, true, true);
    }
    else
    {
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

auto MidiManager::max_index() const -> int
{
    auto const it = std::max_element(mIndexToValue.begin(), mIndexToValue.end(), [](auto const& p1, auto const& p2) {
        return p1.first < p2.first; // Compare the indices
    });

    if (it == mIndexToValue.end())
        return 0;
    return it->first;
}

void MidiManager::imgui_emulate_midi_keyboard()
{
    static auto values = std::vector<float>(31);
    for (int i = 0; i < static_cast<int>(values.size()); ++i)
    {
        ImGui::PushID(i);
        if (ImGui::VSliderFloat("", ImVec2(30, 160), &values[i], 0.f, 1.f, ""))
        {
            std::vector<unsigned char> message{0, static_cast<unsigned char>(i), static_cast<unsigned char>(values[i] * 127)};
            midiCallback(0.f, &message, this);
        }
        ImGui::SetItemTooltip("%i", i);
        if (i != static_cast<int>(values.size()) - 1)
            ImGui::SameLine();
        ImGui::PopID();
    }
}

void MidiManager::imgui_visualize_channels()
{
    ImGui::Text(buf);
    auto values = std::vector<float>(max_index() + 1);
    for (int i = 0; i < static_cast<int>(values.size()); ++i)
        values[i] = get_value({i});

    ImGui::PlotHistogram("Channels", values.data(), static_cast<int>(values.size()), 0, nullptr, 0.f, 1.f, ImVec2(0, 80.0f));
}

} // namespace Cool