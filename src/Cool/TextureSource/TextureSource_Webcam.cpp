#include "TextureSource_Webcam.h"
#include <wcam/wcam.hpp>
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Webcam/WebcamImage.hpp"
#include "Cool/Webcam/WebcamsConfigs.hpp"
#include "TextureLibrary_Webcam.hpp"

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

    if (ImGuiExtras::button_with_text_icon(ICOMOON_COG))
        WebcamsConfigs::instance().open_imgui_window();
    ImGui::SetItemTooltip("%s", "Open webcam config to choose its resolution. This can help if the refresh rate of your webcam is too slow.");

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
    auto       img         = std::shared_ptr<wcam::Image const>{};
    std::visit(
        wcam::overloaded{
            [&](std::shared_ptr<wcam::Image const> const& image) {
                img = image;
            },
            [&](wcam::CaptureError const& error) {
                _error_message = wcam::to_string(error);
            },
            [&](wcam::NoNewImageAvailableYet) {
                _error_message.reset();
            },
            [&](wcam::ImageNotInitYet) {
                _error_message.reset();
            }
        },
        maybe_image
    );
    if (img == nullptr)
        return nullptr;
    TextureLibrary_Webcam::instance().keep_image_alive_this_frame(img); // Otherwise the image can get freed, apparently sometimes the TextureSource_Webcam get destroyed before we render the image, so storing the image in the TextureSource wouldn't solve it
    return &static_cast<WebcamImage const*>(img.get())->get_texture();
}

// TODO(Webcam) When the webcam node is not used in the graph, we must release the SharedWebcam to stop capturing it

auto TextureSource_Webcam::get_error() const -> std::optional<std::string>
{
    return _error_message;
}

} // namespace Cool