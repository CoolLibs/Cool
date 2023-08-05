#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Cool/Path/Path.h>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <webcam_info/webcam_info.hpp>
#include "Cool/Gpu/Texture.h"
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Log/MessageId.h"
#include "Cool/Serialization/SerializerOnDemand.h"

namespace Cool {

struct WebcamConfig {
    webcam_info::Resolution resolution;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Width", resolution.width),
            cereal::make_nvp("Height", resolution.height)
        );
    }
};

using WebcamsConfigsList = std::unordered_map<std::string, WebcamConfig>;

struct WebcamCapture {
    WebcamCapture() = default;

    explicit WebcamCapture(size_t index)
        : _webcam_index(index), _thread{&WebcamCapture::thread_webcam_work, std::ref(*this), index}
    {
    }

    Cool::MessageId _iderrorme_opencv;
    Cool::MessageId _iderrorme_is_not_open;

    std::optional<size_t>        _webcam_index;
    bool                         is_dirty = true;
    std::optional<Cool::Texture> _texture{};
    std::mutex                   _mutex;
    std::jthread                 _thread;
    cv::Mat                      _available_image{};

    std::optional<std::string> error{};

    static void thread_webcam_work(const std::stop_token& stop_token, WebcamCapture& This, size_t webcam_index);
};

struct WebcamRequest {
    explicit WebcamRequest(std::optional<size_t> const& index, std::string name)
        : _name(name)
    {
        if (index.has_value())
            create_capture(*index);
    }

    void create_capture(size_t index);

    std::string                    _name;
    bool                           has_been_requested_this_frame = true;
    Cool::MessageId                _iderror_cannot_find_webcam;
    std::unique_ptr<WebcamCapture> _capture;
};

void update_webcam_ifn(WebcamCapture& webcam);

class TextureLibrary_FromWebcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_FromWebcam&;

    auto get_request(std::string name) -> WebcamRequest*;
    auto get_index_from_name(std::string name) -> std::optional<size_t>;
    auto get_name_from_index(size_t index) -> std::optional<std::string>;
    auto get_resolution_from_index(size_t index) -> std::optional<webcam_info::Resolution>;
    auto get_resolution_from_name(const std::string& name) -> webcam_info::Resolution;
    auto get_default_resolution_from_name(const std::string& name) -> std::optional<webcam_info::Resolution>;
    auto get_webcam_texture(std::string name) -> Texture const*;
    void on_frame_begin(); // TODO(TD)(à test) remet tous les is_dirty à true
    void on_frame_end();   // TODO(TD)(à test) supprime toutes les texture qui sont dirty (car elles n'ont pas été utilisées à cette frame)

    auto imgui_widget_webcam_name(std::string& webcam_name) -> bool;
    void open_webcams_config_window();

    auto get_config_from_name(const std::string& name) -> WebcamConfig&;
    auto get_request_from_name(const std::string& name) -> WebcamRequest*;

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
    static void thread_webcams_infos_works(const std::stop_token& stop_token, TextureLibrary_FromWebcam& This);

    static auto gen_instance() -> TextureLibrary_FromWebcam&;

private:
    std::vector<WebcamRequest>     _requests;
    std::vector<webcam_info::Info> _webcams_infos;
    WebcamsConfigsList             _list_webcam_configs{};

    std::jthread _thread_webcam_infos;
    std::mutex   _mutex_webcam_info;

    ImGuiWindow              _webcam_config_window{icon_fmt("Webcams Config", ICOMOON_COG)};
    Cool::SerializerOnDemand _serializer{Cool::Path::user_data() / "webcams-configs.json", "Configs"};
};

} // namespace Cool