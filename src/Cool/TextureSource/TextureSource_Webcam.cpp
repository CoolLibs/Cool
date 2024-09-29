#include "TextureSource_Webcam.h"
#include <wcam/wcam.hpp>
#include "Cool/Webcam/WebcamImage.hpp"

namespace Cool {

void TextureSource_Webcam::set_device_id(wcam::DeviceId const& id) const
{
    _device_id = id;
    _webcam    = wcam::open_webcam(_device_id);
}

auto TextureSource_Webcam::imgui_widget() -> bool
{
    bool       b            = false;
    auto const webcam_infos = wcam::all_webcams_info();
    if (_device_id == wcam::DeviceId{} && !webcam_infos.empty())
        set_device_id(webcam_infos[0].id);

    auto const it   = std::find_if(webcam_infos.begin(), webcam_infos.end(), [&](wcam::Info const& info) {
        return info.id == _device_id;
    });
    auto const name = it != webcam_infos.end() ? it->name : ""s;

    if (ImGui::BeginCombo("Webcam", name.c_str()))
    {
        for (auto const& info : webcam_infos)
        {
            bool const is_selected = info.id == _device_id;
            if (ImGui::Selectable(info.name.c_str(), is_selected))
            {
                set_device_id(info.id);
                b = true;
            }
            if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    return b;
}

auto TextureSource_Webcam::get_texture() const -> Texture const*
{
    if (_device_id == wcam::DeviceId{})
    {
        auto const webcam_infos = wcam::all_webcams_info();
        if (!webcam_infos.empty())
            set_device_id(webcam_infos[0].id);
    }
    if (!_webcam.has_value())
        _webcam = wcam::open_webcam(_device_id);

    auto const maybe_image = _webcam->image();
    std::visit(
        wcam::overloaded{
            [&](std::shared_ptr<wcam::Image const> const& image) {
                _image = image;
            },
            [&](wcam::CaptureError const& error) {
                _error_message = wcam::to_string(error);
            },
            [&](wcam::NoNewImageAvailableYet) {
                _error_message.reset();
            },
            [&](wcam::ImageNotInitYet) {
                _error_message.reset();
                _image = nullptr; // Reset the image, otherwise it will show briefly when opening the next webcam (while the new capture hasn't returned any image yet) / when a capture needs to restart because the camera was unplugged and then plugged back
            }
        },
        maybe_image
    );
    if (_image == nullptr)
        return nullptr;
    return &static_cast<WebcamImage const*>(_image.get())->get_texture();
}

auto TextureSource_Webcam::get_error() const -> std::optional<std::string>
{
    return _error_message;
}

} // namespace Cool