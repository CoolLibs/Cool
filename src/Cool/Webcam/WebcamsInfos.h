#pragma once
#include <webcam_info/webcam_info.hpp>

namespace Cool {

class WebcamsInfos {
public:
    [[nodiscard]] static auto instance() -> WebcamsInfos&;
    auto                      get_index_from_name(std::string name) -> std::optional<size_t>;
    auto                      get_name_from_index(size_t index) -> std::optional<std::string>;
    [[nodiscard]] auto        get_default_webcam_name() -> std::string;
    auto                      get_default_resolution_from_name(const std::string& name) -> std::optional<webcam_info::Resolution>;
    auto                      imgui_widget_webcam_name(std::string& webcam_name) -> bool;

    void for_each_webcam_info(std::function<void(webcam_info::Info const&)> const&);

private:
    WebcamsInfos() // This is a singleton. Get the global instance with `instance()` instead.
        : _thread_webcam_infos(&WebcamsInfos::thread_webcams_infos_works, std::ref(*this))
    {}
    static void thread_webcams_infos_works(std::stop_token const& stop_token, WebcamsInfos& This);

private:
    std::vector<webcam_info::Info> _webcams_infos;
    std::jthread                   _thread_webcam_infos;
    std::mutex                     _mutex_webcam_info;
};

} // namespace Cool