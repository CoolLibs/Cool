#include "TextureLibrary_Webcam.hpp"
#include <wcam/wcam.hpp>
#include "Cool/Webcam/WebcamImage.hpp"

namespace Cool {

auto TextureLibrary_Webcam::instance() -> TextureLibrary_Webcam&
{
    static auto inst = TextureLibrary_Webcam{};
    return inst;
}

void TextureLibrary_Webcam::on_frame_end()
{
    std::erase_if(_webcams, [&](WebcamData const& data) {
        return !data.has_been_requested_this_frame;
    });
    for (auto& data : _webcams)
        data.has_been_requested_this_frame = false;
}

auto TextureLibrary_Webcam::get_texture(wcam::DeviceId const& id) -> Texture const*
{
    auto it = std::find_if(_webcams.begin(), _webcams.end(), [&](WebcamData const& data) {
        return data.webcam.id() == id;
    });
    if (it == _webcams.end())
    {
        _webcams.emplace_back(wcam::open_webcam(id));
        it = std::prev(_webcams.end());
    }
    auto& webcam_data = *it;
    if (!webcam_data.has_been_requested_this_frame)
    {
        webcam_data.has_been_requested_this_frame = true;
        std::visit(
            wcam::overloaded{
                [&](std::shared_ptr<wcam::Image const> const& image) {
                    webcam_data.image = image;
                    webcam_data.error_message.reset();
                },
                [&](wcam::CaptureError const& error) {
                    webcam_data.image         = nullptr;
                    webcam_data.error_message = wcam::to_string(error);
                },
                [&](wcam::NoNewImageAvailableYet) {
                    webcam_data.error_message.reset();
                },
                [&](wcam::ImageNotInitYet) {
                    webcam_data.image = nullptr;
                    webcam_data.error_message.reset();
                }
            },
            webcam_data.webcam.image()
        );
    }
    if (webcam_data.image == nullptr)
        return nullptr;
    return &static_cast<WebcamImage const*>(webcam_data.image.get())->get_texture(); // NOLINT(*static-cast-downcast)
}

auto TextureLibrary_Webcam::get_error(wcam::DeviceId const& id) const -> std::optional<std::string>
{
    auto it = std::find_if(_webcams.begin(), _webcams.end(), [&](WebcamData const& data) {
        return data.webcam.id() == id;
    });
    if (it == _webcams.end())
        return std::nullopt;
    return it->error_message;
}

void TextureLibrary_Webcam::shut_down()
{
    _webcams.clear();
}

} // namespace Cool