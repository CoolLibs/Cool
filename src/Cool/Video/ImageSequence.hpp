#pragma once
#include "Cool/File/File.h"
#include "Cool/Gpu/Texture.h"
#include "Cool/Time/Time.hpp"

namespace Cool {

class ImageSequence {
public:
    auto get_texture(Time const& time) const -> Texture const*;

    void set_folder(std::filesystem::path const&);

    auto        imgui() -> bool;
    friend auto operator==(ImageSequence const& a, ImageSequence const& b) -> bool
    {
        return Cool::File::equivalent(a._folder_path, b._folder_path);
    }

private:
    std::filesystem::path              _folder_path{};
    std::vector<std::filesystem::path> _images_paths{};

    float framerate{30.f}; // fps
};

} // namespace Cool