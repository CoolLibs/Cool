#pragma once

#include <chrono>
#include <string>
#include <vector>
#include "cereal/types/chrono.hpp"

namespace Cool {
class DidYouKnowModal {
public:
    void open();

    void open_ifn();

    void imgui_windows();

    static void imgui_all_tips();

private:
    void prepare_next_tip();

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
    bool _is_open       = false;
    bool _show_all_tips = false;

    size_t _current_tip_index = 0;

    std::chrono::steady_clock::time_point _timestamp_last_opening = std::chrono::steady_clock::now();

    friend void debug_did_you_know(Cool::DidYouKnowModal& _did_you_know);
};

} // namespace Cool

const std::chrono::hours timeToWait = 1h;