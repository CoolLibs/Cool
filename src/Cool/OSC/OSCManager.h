#pragma once
#include "Cool/ImGui/ImGuiWindow.h"

namespace Cool {

class OSCManager {
public:
    void open_config_window() { _config_window.open(); }
    void imgui_window_config();

private:
    OSCManager(); // This is private, because all instances of OSCManager must bu guaranteed to have a fixed address in memory. Use osc_manager() to access the global OSCManager.
    friend auto osc_manager() -> OSCManager&;

    void start_listening();
    void stop_listening();

    void imgui_show_all_values();

private:
    std::mutex                                 _mutex;
    std::vector<std::pair<std::string, float>> _values{};
    Cool::ImGuiWindow                          _config_window;
    std::optional<std::thread>                 _thread;

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

inline auto osc_manager() -> OSCManager&
{
    static auto instance = OSCManager{};
    return instance;
}

} // namespace Cool