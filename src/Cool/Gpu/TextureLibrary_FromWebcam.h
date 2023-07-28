#pragma once
#include <vcruntime.h>
#include <array>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <optional>
#include <stop_token>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"
#include "Cool/Log/ToUser.h"

namespace Cool {

struct WebcamCapture {
    WebcamCapture() = default;
    explicit WebcamCapture(int id)
        : _name{"Unknown TODO(TD) " + std::to_string(id)}
        , _webcam_id(id)
        , _thread{&WebcamCapture::thread_webcam_work, std::ref(*this), id}
    {
    }
    std::optional<Cool::Texture> _texture{};
    std::string                  _name{};
    bool                         is_dirty                      = true;
    bool                         has_been_requested_this_frame = true;

    int          _webcam_id{};
    cv::Mat      _available_image{};
    std::mutex   _mutex;
    std::jthread _thread;

    static void thread_webcam_work(std::stop_token stop_token, WebcamCapture& This, int webcam_id)
    {
        try
        {
            cv::VideoCapture capture{webcam_id};
            cv::Mat          wip_image{};

            int width  = 1920;
            int height = 1080;
            capture.set(cv::CAP_PROP_FRAME_WIDTH, width);
            capture.set(cv::CAP_PROP_FRAME_HEIGHT, height);

            while (!stop_token.stop_requested())
            {
                capture >> wip_image;
                std::cout << capture.isOpened() << "\n";
                {
                    std::scoped_lock lock(This._mutex);
                    cv::swap(This._available_image, wip_image);
                    This.is_dirty = true;
                }
            }
        }
        catch (cv::Exception& e)
        {
            Cool::Log::ToUser::console()
                .send(
                    Message{
                        .category = "Nodes",
                        .message  = fmt::format("OpenCV error : {}", e.what()),
                        .severity = MessageSeverity::Warning,
                    }
                );
        }
    }
};

void update_webcam_ifn(WebcamCapture& webcam);

class TextureLibrary_FromWebcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_FromWebcam&
    {
        static auto inst = TextureLibrary_FromWebcam{};
        return inst;
    }
    auto get_webcam(int i) -> WebcamCapture*;
    auto get_webcam_texture(int index) -> std::optional<Texture> const&;
    void on_frame_begin(); // TODO(TD)(à test) remet tous les is_dirty à true
    void on_frame_end();   // TODO(TD)(à test) supprime toutes les texture qui sont dirty (car elles n'ont pas été utilisées à cette frame)

    auto imgui_widget_webcam_index(int& webcam_index) -> bool;

    [[nodiscard]] auto has_active_webcam() const -> bool;
    [[nodiscard]] auto error_from(int index) const -> std::optional<std::string>;

private:
    TextureLibrary_FromWebcam() = default;
    ; // This is a singleton. Get the global instance with `instance()` instead.

    auto find_next_webcam_index(int start_index) -> int; // TODO(TD) autre thread en boucle ; dès qu'on détecte que le nb webcam a changé, on supprime toutes celles existantes et on recrée tout
    void add_webcam(int id);
    void update_webcams();

    void get_number_webcam_win();

private:
    std::list<WebcamCapture> _webcams;
};

} // namespace Cool