#pragma once
#include "MidiCc.h"

namespace Cool {

    class MidiManager{
        public:
auto get_value(MidiCc const& ) -> float ;

private:
// TODO(Midi) map index vers float
    };

   inline auto midi_manager() -> MidiManager&
    {
        static auto instance = MidiManager{};
        return instance;
    }

}