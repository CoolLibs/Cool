#pragma once
#include <vcruntime.h>
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <string>
#include <thread>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"

namespace Cool {

struct WebcamCaptureThread {
    int          _capture_id{};
    std::jthread _thread{&WebcamCaptureThread::thread_webcam_work, this};
    // std::jthread     _thread{};

    void thread_webcam_work()
    {
        cv::VideoCapture capture{0};
        cv::Mat          _mat_texture{};
        int              vallll = 0;

        while (true)
        {
            capture >> _mat_texture;
            vallll++;

            // const auto width  = static_cast<unsigned int>(mat.cols);
            // const auto height = static_cast<unsigned int>(mat.rows);

            // _thread_texture.bind();
            // _thread_texture.set_image(
            //     {width, height},
            //     3, reinterpret_cast<uint8_t*>(mat.ptr())
            // );
            // Cool::Texture::unbind();
        }
    }
};

struct WebcamCapture {
    std::optional<Cool::Texture> _texture{};
    cv::VideoCapture             _capture{};
    std::string                  _name{};
    bool                         is_dirty = true;
    WebcamCaptureThread          _webcam_thread{};
};

void update_webcam(WebcamCapture& webcam);

class TextureLibrary_FromWebcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_FromWebcam&
    {
        static auto inst = TextureLibrary_FromWebcam{};
        return inst;
    }

    auto get_webcam_texture(size_t index) -> std::optional<Texture> const&;
    void on_frame_begin(); // TODO(TD)(à test) remet tous les is_dirty à true
    void on_frame_end();   // TODO(TD)(à test) supprime toutes les texture qui sont dirty (car elles n'ont pas été utilisées à cette frame)

    auto imgui_widget_webcam_index(int& webcam_index) -> bool;

    auto has_active_webcam() const -> bool;
    auto error_from(const size_t index) const -> std::optional<std::string>;

private:
    TextureLibrary_FromWebcam(); // This is a singleton. Get the global instance with `instance()` instead.

    auto compute_number_of_camera() -> int; // TODO(TD) autre thread en boucle ; dès qu'on détecte que le nb webcam a changé, on supprime toutes celles existantes et on recrée tout
    void add_webcam();
    void update_webcams();

private:
    int                        _number_of_webcam{};
    std::vector<WebcamCapture> _list_webcam{};
};

} // namespace Cool