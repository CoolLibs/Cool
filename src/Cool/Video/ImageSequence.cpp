#include "ImageSequence.hpp"
#include <imgui.h>
#include <cstddef>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/TextureSource/TextureLibrary_Image.h"

namespace Cool {

// TODO handlle looping options (like video player) :
//      add a VideoPlayerSettings (and show it in the gui)
//      take the code from internal::CaptureState::get_texture that applies the Settings to the Time; move it to a separate function, and reuse it in image sequence
//      for the case where we return transparent texture, return a time of -1, and then check (in video player and image sequence, if the time is < 0 tehen return transparent texture)
// TODO gui for framerate

// TODO lo  d images when opening project file
// TODO rerender when frame changes
// TODO log error
// TODO improve perf

// TODO(Jules) addig a variable broke serialization

auto ImageSequence::get_texture(Time const& time) const -> Texture const*
{
    int frame = static_cast<int>(time.as_seconds_float() * framerate);
    if (frame > static_cast<int>(_images_paths.size()) - 1 || frame < 0)
    {
        return nullptr;
    }
    else
    {
        return TextureLibrary_Image::instance().get(_images_paths[frame], 1s);
    }
}

auto ImageSequence::imgui() -> bool
{
    bool b = false;
    b |= ImGuiExtras::folder("Image Path", &_folder_path);
    if (b)
        set_folder(_folder_path);
    ImGui::InputFloat("##FPS", &framerate, 0.f, 0.f, "%.2f fps");

    return b;
}

void ImageSequence::set_folder(std::filesystem::path const& path)
{
    _folder_path = path;

    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(_folder_path))
        {
            _images_paths.push_back(entry);
        }
    }
    catch (...)
    {
    }
}

} // namespace Cool