#include "ExportSize.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <smart/smart.hpp>
#include <stringify/stringify.hpp>

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
            _size.width() / _aspect_ratio.get()
        ));
    }
    else
    {
        _size.set_width(static_cast<decltype(_size)::DataType>(
            _size.height() * _aspect_ratio.get()
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
    ImGui::BeginDisabled(!_aspect_ratio_is_locked);
    b |= _aspect_ratio.imgui(150.f);
    ImGui::EndDisabled();

    ImGui::SameLine();
    b |= ImGui::Checkbox("##lock_ratio", &_aspect_ratio_is_locked);
    ImGuiExtras::tooltip("Lock the aspect ratio");

    if (b && _aspect_ratio_is_locked)
    {
        apply_aspect_ratio();
    }

    return b;
}

} // namespace Cool