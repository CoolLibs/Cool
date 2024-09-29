#pragma once
#include <optional>
#include <string>
#include <wcam/wcam.hpp>
#include "Cool/Gpu/Texture.h"

namespace Cool {

class TextureSource_Webcam {
public:
    auto               imgui_widget() -> bool;
    [[nodiscard]] auto get_texture() const -> Texture const*;
    [[nodiscard]] auto get_error() const -> std::optional<std::string>;

    friend auto operator==(TextureSource_Webcam const& a, TextureSource_Webcam const& b) -> bool
    {
        return a._device_id == b._device_id;
    }

private:
    void set_device_id(wcam::DeviceId const& id) const;

private:
    mutable wcam::DeviceId                     _device_id{/* TODO(Webcam) Cool::default_webcam_name() */}; // TODO(Webcam) update this comment // NB: most of the time a Source is created too early for this to have any effect (the thread that finds the webcam's names hasn't had time to do its job yet). This is why we have a HACK in TextureLibrary_FromWebcam::imgui_widget_webcam_name to still init with a proper webcam name.
    mutable std::optional<wcam::SharedWebcam>  _webcam{};
    mutable std::shared_ptr<wcam::Image const> _image{};
    mutable std::optional<std::string>         _error_message{};
    wcam::KeepLibraryAlive                     _keep_wcam_library_alive{};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            // ser20::make_nvp("Device ID", _device_id) // TODO(Webcam)
        );
    }
};

} // namespace Cool