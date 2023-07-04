#pragma once
#include <vcruntime.h>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <string>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

namespace Cool {

struct WebcamCapture {
    std::optional<Cool::Texture> _texture{};
    cv::VideoCapture             _capture{};
    std::string                  _name{};
    bool                         is_dirty = true;
    // TODO(TD) check si la capture a été request / si non => détruire la texture
};

void update_webcam(WebcamCapture& webcam);

class TextureLibrary_FromWebcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_FromWebcam&
    {
        static auto inst = TextureLibrary_FromWebcam{};
        return inst;
    }

    auto get_webcam_texture(size_t index) -> Texture const&;
    void on_frame_begin(); // TODO(TD) remet tous les is_dirty à true
    void on_frame_end();   // TODO(TD) supprime toutes les texture qui sont dirty (car elles n'ont pas été utilisées à cette frame)

    auto imgui_widget_webcam_index(int& webcam_index) -> bool;

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