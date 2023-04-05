#pragma once

#include <chrono>
#include <string>
#include <vector>
#include "cereal/types/chrono.hpp"
class DidYouKnowModal {
public:
    void open();

    void imgui_window();

private:
    bool _has_been_opened = false;

    size_t _current_tip_index = 0;

    std::chrono::steady_clock::time_point _timestamp_last_opening;

    void prepare_next_tip();

    static void all_tips();

    friend void debug_did_you_know(DidYouKnowModal& _did_you_know);

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Last tip", _current_tip_index),
            cereal::make_nvp("Last timestamp", _timestamp_last_opening)
        );
    }
};

void test_did_you_know(DidYouKnowModal& _did_you_know);