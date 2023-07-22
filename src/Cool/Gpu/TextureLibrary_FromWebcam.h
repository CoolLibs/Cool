#pragma once
#include <vcruntime.h>
#include <array>
#include <functional>
#include <memory>
#include <mutex>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"

namespace Cool {

struct WebcamCapture {
    std::optional<Cool::Texture> _texture{};
    std::string                  _name{};
    bool                         is_dirty = true;

    int          _webcam_id{};
    cv::Mat      _available_image{};
    std::mutex   _mutex;
    std::jthread _thread{&WebcamCapture::thread_webcam_work, this};

    void thread_webcam_work()
    {
        cv::VideoCapture capture{_webcam_id};
        cv::Mat          wip_image{};
        while (true)
        {
            capture >> wip_image;
            {
                std::scoped_lock lock(_mutex);
                cv::swap(_available_image, wip_image);
            }
        }
    }
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
    int                                    _number_of_webcam{};
    std::unordered_map<int, WebcamCapture> _webcams{};
};

} // namespace Cool