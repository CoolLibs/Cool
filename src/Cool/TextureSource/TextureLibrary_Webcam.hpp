#pragma once
#include "wcam/wcam.hpp"

namespace Cool {

class TextureLibrary_Webcam {
public:
    [[nodiscard]] static auto instance() -> TextureLibrary_Webcam&;

    void keep_image_alive_this_frame(std::shared_ptr<wcam::Image const> const&);

private:
    friend class AppManager;
    void on_frame_end();

private:
    TextureLibrary_Webcam() = default; // This is a singleton. Get the global instance with `instance()` instead.

private:
    std::vector<std::shared_ptr<wcam::Image const>> _images_to_keep_alive_for_this_frame{};
};

} // namespace Cool