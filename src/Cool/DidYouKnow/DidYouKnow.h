#pragma once
#include <chrono>
#include <string>
#include <vector>
#include "Cool/DidYouKnow/DidYouKnow.h"
#include "cereal/types/chrono.hpp"

namespace Cool {

using Tips = std::span<const char* const>;

/// A class that manages the "Did you know" modal. It is used to display tips to the user at the opening of the software.
/// Also it is possible to display all the tips at once.
class DidYouKnow {
public:
    void open_popup(Tips);
    void imgui_windows(Tips);

private:
    void open_ifn(Tips);
    void prepare_next_tip(Tips);

    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Current tip", _current_tip_index),
            cereal::make_nvp("Last opened", _timestamp_last_opening)
        );
    }

private:
    bool _app_has_just_been_opened = true;
    bool _is_open                  = false;
    bool _show_all_tips            = false;

    size_t _current_tip_index = 0;

    std::chrono::steady_clock::time_point _timestamp_last_opening = std::chrono::steady_clock::now();

    friend void test_did_you_know(Cool::DidYouKnow&, Tips); // To display the internal values like remaining time before opening.
};

} // namespace Cool
