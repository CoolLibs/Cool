#pragma once
#include <Cool/Image/AspectRatio.h>
#include <Cool/Image/ImageSizeU.h>
#include <img/img.hpp>

namespace Cool {

class ExportSize {
public:
    operator img::Size() { return _size; }

    auto imgui() -> bool;
    void set_aspect_ratio(AspectRatio aspect_ratio); // Don't just take a float as input because we want to keep the combo index in the ImGui dropdown (e.g. remember if we were exactly 16/9, 3/2, etc.)

private:
    void apply_aspect_ratio();

private:
    img::Size      _size{1920, 1080};
    bool           _aspect_ratio_is_locked{true};
    AspectRatio    _aspect_ratio{};
    ImageSizeU::WH _last_changed_side{ImageSizeU::WH::None};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Size", _size),
            cereal::make_nvp("Aspect Ratio is Locked", _aspect_ratio_is_locked),
            cereal::make_nvp("Aspect Ratio", _aspect_ratio)
        );
    }
};

} // namespace Cool