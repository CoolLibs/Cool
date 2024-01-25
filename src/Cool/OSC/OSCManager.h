#pragma once
#include <set>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/OSC/OSCChannel.h"
#include "OSCChannel.h"
#include "OSCConnectionEndpoint.h"

namespace Cool {

namespace internal {
struct SharedWithThread {
    mutable std::mutex                         values_mutex{};
    std::vector<std::pair<std::string, float>> values{};
    std::mutex                                 channels_that_have_changed_mutex{};
    std::set<OSCChannel>                       channels_that_have_changed{};
};
} // namespace internal

class OSCManager {
public:
    OSCManager(OSCManager const&)                = delete;
    OSCManager& operator=(OSCManager const&)     = delete;
    OSCManager(OSCManager&&) noexcept            = delete;
    OSCManager& operator=(OSCManager&&) noexcept = delete;
    ~OSCManager();

    auto get_value(OSCChannel const&) const -> float;
    /// The `callback` will be called once for each channel whose value has changed since the last call to `for_each_channel_that_has_changed()`.
    void for_each_channel_that_has_changed(std::function<void(OSCChannel const&)> const& callback);

    void set_port(int port);
    void set_ip_address(std::string ip_address);
    void set_connection_endpoint(OSCConnectionEndpoint);
    auto get_connection_endpoint() const -> OSCConnectionEndpoint;

    auto imgui_channel_widget(const char* label, OSCChannel&) const -> bool;

    void open_config_window() { _config_window.open(); }
    void imgui_window_config();

private:
    OSCManager(); // This is private, because all instances of OSCManager must be guaranteed to have a fixed address in memory. Use osc_manager() to access the global OSCManager.
    friend auto osc_manager() -> OSCManager&;

    void start_listening();
    void stop_listening();

    void imgui_show_all_values();
    void imgui_select_connection_endpoint();

private:
    internal::SharedWithThread _s{};
    Cool::ImGuiWindow          _config_window;
    std::optional<std::thread> _thread;
    std::function<void()>      _stop_thread;
    OSCConnectionEndpoint      _endpoint{};

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