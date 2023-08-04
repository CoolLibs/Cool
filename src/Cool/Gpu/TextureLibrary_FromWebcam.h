#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
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
#include <ostream>
#include <stop_token>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <webcam_info/webcam_info.hpp>
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureLibrary_FromWebcam.h"
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Log/MessageId.h"
#include "Cool/Log/ToUser.h"

namespace Cool {

struct WebcamCapture {
    WebcamCapture() = default;

    explicit WebcamCapture(int index)
        : _webcam_index(index), _thread{&WebcamCapture::thread_webcam_work, std::ref(*this), index}
    {
    }

    Cool::MessageId _iderrorme_opencv;
    Cool::MessageId _iderrorme_is_not_open;

    std::optional<int>           _webcam_index;
    bool                         is_dirty = true;
    std::optional<Cool::Texture> _texture{};
    std::mutex                   _mutex;
    std::jthread                 _thread;
    cv::Mat                      _available_image{};

    std::optional<std::string> error{};

    static void thread_webcam_work(const std::stop_token& stop_token, WebcamCapture& This, int webcam_index);
};

struct WebcamRequest {
    explicit WebcamRequest(std::optional<int> const& index, std::string name)
        : _name(name)
    {
        if (index.has_value())
            create_capture(*index);
    }

    void create_capture(int index);

    std::string                    _name;
    bool                           has_been_requested_this_frame = true;
    Cool::MessageId                _iderror_cannot_find_webcam;
    std::unique_ptr<WebcamCapture> _capture;
};

void update_webcam_ifn(WebcamCapture& webcam);

class TextureLibrary_FromWebcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_FromWebcam&
    {
        static auto inst = TextureLibrary_FromWebcam();
        return inst;
    }
    auto get_request(std::string name) -> WebcamRequest*;
    auto get_index_from_name(std::string name) -> std::optional<int>;
    auto get_name_from_index(int index) -> std::optional<std::string>;
    auto get_resolution_from_index(int index) -> std::optional<std::pair<int, int>>;
    auto get_webcam_texture(std::string name) -> std::optional<Texture> const&;
    void on_frame_begin(); // TODO(TD)(à test) remet tous les is_dirty à true
    void on_frame_end();   // TODO(TD)(à test) supprime toutes les texture qui sont dirty (car elles n'ont pas été utilisées à cette frame)

    auto imgui_widget_webcam_name(std::string& webcam_name) -> bool;
    void open_webcams_config_window();

    [[nodiscard]] auto has_active_webcam() const -> bool;
    [[nodiscard]] auto error_from(std::string webcam_name) const -> std::optional<std::string>;

    [[nodiscard]] auto get_default_webcam_name() -> std::string;

    void imgui_windows();

private:
    TextureLibrary_FromWebcam()
        : _thread_webcam_infos(&TextureLibrary_FromWebcam::thread_webcams_infos_works, std::ref(*this)){};
    // : _thread_webcam_infos{} {}; // This is a singleton. Get the global instance with `instance()` instead.

    // auto find_next_webcam_index(int start_index) -> int; // TODO(TD) autre thread en boucle ; dès qu'on détecte que le nb webcam a changé, on supprime toutes celles existantes et on recrée tout
    // void add_webcam(int id, std::string name);
    void update_webcams();

    void        get_number_webcam_win();
    static void thread_webcams_infos_works(const std::stop_token& stop_token, TextureLibrary_FromWebcam& This)
    {
        std::vector<webcam_info::info> wip_webcams_infos{};
        while (!stop_token.stop_requested())
        {
            wip_webcams_infos = webcam_info::get_all_webcams();
            {
                std::scoped_lock<std::mutex> lock(This._mutex_webcam_info);
                std::swap(wip_webcams_infos, This._webcams_infos);
            }
        }

    } // namespace Cool

private:
    std::vector<WebcamRequest>     _requests;
    std::vector<webcam_info::info> _webcams_infos;

    std::jthread _thread_webcam_infos;
    std::mutex   _mutex_webcam_info;

    ImGuiWindow _webcam_config_window{icon_fmt("Webcams Config", ICOMOON_COG)};
};

} // namespace Cool