#pragma once
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include "Cool/ImGui/ImGuiWindow.h"
#include "MidiChannel.h"
#include "RtMidiW/RtMidiW.hpp"

namespace Cool {

class MidiManager {
public:
    /// Returns a number between 0 and 1 representing the current state of the controller.
    /// /!\ Note that if a knob hasn't been used yet we won't know it's current state and will return 0 instead.
    [[nodiscard]] auto get_value(MidiChannel const&) const -> float;
    /// The `callback` will be called once for each channel whose value has changed since the last call to `for_each_channel_that_has_changed()`.
    void for_each_channel_that_has_changed(std::function<void(MidiChannel const&)> const& callback);

    /// You can use this if you want to emulate a midi keyboard.
    /// `value` should be between 0 and 1, since this is what people expect to receive back from `get_value()`.
    void set_value(MidiChannel const&, float value);

    void open_config_window() { _config_window.open(); }

    void imgui_window();
    void imgui_visualize_channels();
    void imgui_controllers_dropdown();
    void imgui_emulate_midi_keyboard();

private:
    friend auto midi_manager() -> MidiManager&;
    MidiManager(); // Use `mini_manager()` to get the unique instance of MidiManager.

    friend class AppManager;
    void check_for_devices();

    void open_port(unsigned int index);
    void close_port();

    [[nodiscard]] auto max_index() const -> int;

    static void midi_callback(double delta_time, std::vector<unsigned char>* message, void* user_data);
    static void midi_error_callback(RtMidiError::Type type, std::string const& error_text, void* user_data);

private:
    std::optional<RtMidiIn>        _midi{}; // Optional because the creation might fail and throw an exception: http://www.music.mcgill.ca/~gary/rtmidi/index.html#start
    std::string                    _port_name{};
    std::optional<unsigned int>    _port_index{};
    std::unordered_map<int, float> _value_from_index{};
    Cool::ImGuiWindow              _config_window;
    std::set<MidiChannel>          _channels_that_have_changed{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Window", _config_window)
        );
    }
};

inline auto midi_manager() -> MidiManager&
{
    static auto instance = MidiManager{};
    return instance;
}

} // namespace Cool