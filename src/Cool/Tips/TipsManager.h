#pragma once
#include <chrono>
#include <string>
#include <vector>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Tips/TipsManager.h"
#include "ser20/types/chrono.hpp"

namespace Cool {

using Tips = std::span<const char* const>;

/// A class that manages the "Did you know" modal. It is used to display tips to the user at the opening of the software.
/// Also it is possible to display all the tips at once.
class TipsManager {
public:
    void open_one_tip_window();
    void open_all_tips_window();
    void imgui_show_one_tip(Tips);
    void imgui_windows(Tips);

    void on_app_shutdown();

private:
    void open_ifn();
    void prepare_tip(int difference);
    void prepare_next_tip();
    void prepare_previous_tip();
    auto get_current_tip(Tips all_tips) -> const char*;

private:
    int         _current_tip_index{-1}; // Start at -1 so that the first tip will be at index 0 (because we increment before opening the popup).
    bool        _show_all_tips{false};
    ImGuiWindow _window{Cool::icon_fmt("Did you know?", ICOMOON_BUBBLE), {.is_modal = true}};

    bool                                  _app_has_just_been_opened{true};
    std::chrono::steady_clock::time_point _time_when_app_was_last_closed{};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Current tip", _current_tip_index),
            ser20::make_nvp("Wants to show all tips", _show_all_tips),
            ser20::make_nvp("Time when app was last closed", _time_when_app_was_last_closed)
        );
    }

    friend void test_tips(Cool::TipsManager&); // To display the internal values like remaining time before opening.
};

} // namespace Cool
