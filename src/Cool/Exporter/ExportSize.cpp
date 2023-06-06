#include "ExportSize.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <imgui.h>
#include <smart/smart.hpp>
#include <stringify/stringify.hpp>
#include "Cool/ImGui/IcoMoonCodepoints.h"

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
            static_cast<float>(_size.width()) / _aspect_ratio.get()
        ));
    }
    else
    {
        _size.set_width(static_cast<decltype(_size)::DataType>(
            static_cast<float>(_size.height()) * _aspect_ratio.get()
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

    if (b && !_aspect_ratio_is_locked)
    {
        _aspect_ratio.set(img::SizeU::aspect_ratio(_size));
    }

    ImGui::SameLine();
    ImGuiExtras::disabled_if(!_aspect_ratio_is_locked, "Ratio is not locked, you cannot edit it directly. Lock it with the button on the right.", [&]() {
        b |= _aspect_ratio.imgui(150.f);
    });
    ImGui::SameLine();
    b |= ImGuiExtras::checkbox_button(ICOMOON_LINK, &_aspect_ratio_is_locked);
    ImGuiExtras::help_marker(
        _aspect_ratio_is_locked
            ? "Unlock the ratio to edit the width and the height independently."
            : "Lock the ratio to tie the width and the height together and always keep the given ratio."
    );

    if (b && _aspect_ratio_is_locked)
    {
        apply_aspect_ratio();
    }

    return b;
}

} // namespace Cool