#pragma once

namespace Cool {

class WebcamCapture {
public:
    explicit WebcamCapture(size_t index)
        : _webcam_index{index}
        , _thread{&WebcamCapture::thread_job_webcam_image, std::ref(*this)}
    {}

    [[nodiscard]] auto has_stopped() const -> bool { return _has_stopped; }
    auto               texture() -> Cool::Texture const*;
    [[nodiscard]] auto webcam_index() const -> size_t { return _webcam_index; }

private:
    static void thread_job_webcam_image(std::stop_token const& stop_token, WebcamCapture& This);
    void        update_webcam_ifn();

private:
    std::mutex                   _mutex{};
    std::optional<Cool::Texture> _texture{};
    bool                         _needs_to_create_texture_from_available_image{false};
    cv::Mat                      _available_image{};
    bool                         _has_stopped{false};
    size_t                       _webcam_index;
    std::jthread                 _thread{};
};

} // namespace Cool