#pragma once
#include <webcam_info/webcam_info.hpp>

namespace Cool {

class WebcamsInfos {
public:
    ~WebcamsInfos();
    WebcamsInfos(WebcamsInfos const&)                        = delete;
    auto operator=(WebcamsInfos const&) -> WebcamsInfos&     = delete;
    WebcamsInfos(WebcamsInfos&&) noexcept                    = delete;
    auto operator=(WebcamsInfos&&) noexcept -> WebcamsInfos& = delete;

    [[nodiscard]] static auto instance() -> WebcamsInfos&;

    [[nodiscard]] auto index(std::string const& webcam_name) -> std::optional<size_t>;
    [[nodiscard]] auto name(size_t webcam_index) -> std::optional<std::string>;

    [[nodiscard]] auto default_webcam_name() -> std::string;
    [[nodiscard]] auto default_resolution(std::string const& webcam_name, bool do_lock = true) -> webcam_info::Resolution;

    auto imgui_widget_webcam_name(std::string& webcam_name) -> bool;

    void for_each_webcam_info(std::function<void(webcam_info::Info const&)> const&);

private:
    WebcamsInfos() // This is a singleton. Get the global instance with `instance()` instead.
        : _thread(&WebcamsInfos::thread_job, std::ref(*this))
    {}
    static void thread_job(WebcamsInfos& This);

    [[nodiscard]] auto default_resolution_locking(std::string const& webcam_name) -> webcam_info::Resolution;
    [[nodiscard]] auto default_resolution_non_locking(std::string const& webcam_name) -> webcam_info::Resolution;

private:
    std::vector<webcam_info::Info> _webcams_infos;
    std::mutex                     _mutex;
    std::atomic<bool>              _wants_to_stop_thread{false};
    std::thread                    _thread;
};

} // namespace Cool