#pragma once
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/OSC/OSCChannel.h"
#include "OSCChannel.h"

namespace Cool {

class OSCManager {
public:
    OSCManager(OSCManager const&)                = delete;
    OSCManager& operator=(OSCManager const&)     = delete;
    OSCManager(OSCManager&&) noexcept            = delete;
    OSCManager& operator=(OSCManager&&) noexcept = delete;
    ~OSCManager();

    auto get_value(OSCChannel const&) const -> float;
    void set_port(int port);
    auto get_port() const -> int;

    auto imgui_channel_widget(const char* label, OSCChannel&) const -> bool;

    void open_config_window() { _config_window.open(); }
    void imgui_window_config();

private:
    OSCManager(); // This is private, because all instances of OSCManager must be guaranteed to have a fixed address in memory. Use osc_manager() to access the global OSCManager.
    friend auto osc_manager() -> OSCManager&;

    void start_listening();
    void stop_listening();

    void imgui_show_all_values();
    void imgui_select_port();

private:
    mutable std::mutex                         _mutex;
    std::vector<std::pair<std::string, float>> _values{};
    Cool::ImGuiWindow                          _config_window;
    std::optional<std::thread>                 _thread;
    std::function<void()>                      _stop_thread;
    int                                        _port{-1};

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