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

struct Webcam {
    std::shared_ptr<Cool::Texture> _texture{};
    cv::VideoCapture               _capture{};
    std::string                    _name{};
    bool                           _is_active{};
};

void update_webcam(Webcam& webcam);

class TextureLibrary_FromWebcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_FromWebcam&
    {
        static auto inst = TextureLibrary_FromWebcam{};
        return inst;
    }

    auto get_webcam_texture(size_t index)
    {
        if (index >= _list_webcam.size())
            return _list_webcam[0]._texture;
        return _list_webcam[index]._texture;
    }

    void update();

    auto is_webcam_used() const -> bool { return _number_of_webcam == 0; };

    auto imgui_widget_webcam_index(int& webcam_index) -> bool;

private:
    TextureLibrary_FromWebcam(); // This is a singleton. Get the global instance with `instance()` instead.

    auto compute_number_of_camera() -> int;
    void add_webcam();
    void update_webcams();

private:
    int                 _number_of_webcam{};
    std::vector<Webcam> _list_webcam{};
};

} // namespace Cool