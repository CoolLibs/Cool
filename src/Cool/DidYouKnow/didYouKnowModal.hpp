#pragma once

#include <chrono>
#include <string>
#include <vector>
#include "Cool/DidYouKnow/didYouKnowModal.hpp"
#include "cereal/types/chrono.hpp"

namespace Cool {
/**
 * @brief A class that manages the "Did you know" modal. It is used to display tips to the user at opening of software. Also it is possible to display all the tips at once.
 *
 */
class DidYouKnow {
public:
    void open(std::span<const char* const>& all_tips);

    void open_ifn(std::span<const char* const>& all_tips);

    void imgui_windows(std::span<const char* const>& all_tips);

private:
    void prepare_next_tip(std::span<const char* const>& all_tips);

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
    bool _open_of_coollab = true;
    bool _is_open         = false;
    bool _show_all_tips   = false;

    size_t _current_tip_index = 0;

    std::chrono::steady_clock::time_point _timestamp_last_opening = std::chrono::steady_clock::now();

    friend void test_did_you_know(Cool::DidYouKnow& _did_you_know, std::span<const char* const>& all_tips);
};

void imgui_all_tips(std::span<const char* const>& all_tips);

void update(Cool::DidYouKnow& _did_you_know, std::span<const char* const>& all_tips);

} // namespace Cool

auto timeToWait() -> std::chrono::steady_clock::duration;