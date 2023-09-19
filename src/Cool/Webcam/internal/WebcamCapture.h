#pragma once
#include <Cool/Gpu/Texture.h>
#include <opencv2/core.hpp>
#include <webcam_info/webcam_info.hpp>

namespace Cool {

class WebcamCapture {
public:
    WebcamCapture(size_t index, webcam_info::Resolution resolution)
        : _webcam_index{index}
        , _thread{&WebcamCapture::thread_job, std::ref(*this), resolution}
    {}
    ~WebcamCapture();
    WebcamCapture(WebcamCapture const&)                        = delete;
    auto operator=(WebcamCapture const&) -> WebcamCapture&     = delete;
    WebcamCapture(WebcamCapture&&) noexcept                    = delete;
    auto operator=(WebcamCapture&&) noexcept -> WebcamCapture& = delete;

    auto               texture() -> Cool::Texture const*;
    [[nodiscard]] auto has_stopped() const -> bool { return _has_stopped; }
    [[nodiscard]] auto webcam_index() const -> size_t { return _webcam_index; }

private:
    static void thread_job(WebcamCapture& This, webcam_info::Resolution);
    void        update_webcam_ifn();

private:
    std::mutex                   _mutex{};
    std::optional<Cool::Texture> _texture{};
    bool                         _needs_to_create_texture_from_available_image{false};
    cv::Mat                      _available_image{};
    bool                         _has_stopped{false};
    size_t                       _webcam_index;
    std::atomic<bool>            _wants_to_stop_thread{false};
    std::thread                  _thread{};
};

} // namespace Cool