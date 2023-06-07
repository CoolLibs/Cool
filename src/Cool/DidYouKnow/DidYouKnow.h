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
    void open_popup();
    void imgui_windows(Tips);

private:
    void open_ifn();
    void prepare_next_tip();
    auto get_current_tip(Tips all_tips) -> const char*;

private:
    bool _app_has_just_been_opened = true;
    bool _is_open                  = false;
    bool _show_all_tips            = false;

    size_t _current_tip_index{static_cast<size_t>(-1)}; // Start at -1 so that the first tip will be at index 0 (because we increment before opening the popup).

    std::chrono::steady_clock::time_point _timestamp_last_opening{}; // TODO(JF) Instead, use the timestamp when the app was last closed

    friend void test_did_you_know(Cool::DidYouKnow&); // To display the internal values like remaining time before opening.

private:
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
};

} // namespace Cool
