#include "ExportSize.h"
#include <Cool/ImGui/ImGuiExtras.h>

namespace Cool {

void ExportSize::set_aspect_ratio(AspectRatio aspect_ratio)
{
    _aspect_ratio_is_locked = true;
    _aspect_ratio           = aspect_ratio;
    apply_aspect_ratio();
}

void ExportSize::apply_aspect_ratio()
{
    if (_last_changed_side == ImageSizeU::WH::Width)
    {
        _size.set_height(static_cast<decltype(_size)::DataType>(
            _size.width() / _aspect_ratio.asFloat()
        ));
    }
    else
    {
        _size.set_width(static_cast<decltype(_size)::DataType>(
            _size.height() * _aspect_ratio.asFloat()
        ));
    }
}

auto ExportSize::imgui() -> bool
{
    bool b = false;

    const ImageSizeU::WH changed_side = ImageSizeU::imgui(_size);
    if (changed_side != ImageSizeU::WH::None)
    {
        _last_changed_side = changed_side; // Only update if changed_side != ImageSizeU::WH::None because we want to remember the last side that actually changed.
        b                  = true;
    }

    ImGui::SameLine();
    ImGui::Dummy(ImVec2{10.f, 0.f});
    ImGui::SameLine();

    b |= ImGuiExtras::checkbox_with_submenu("Locked Aspect ratio", &_aspect_ratio_is_locked, [&]() {
        return _aspect_ratio.ImGuiPicker(45);
    });

    if (b && _aspect_ratio_is_locked)
    {
        apply_aspect_ratio();
    }

    return b;
}

} // namespace Cool