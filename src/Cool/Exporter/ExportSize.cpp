#include "ExportSize.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>

namespace Cool {

void ExportSize::set_aspect_ratio(AspectRatio aspect_ratio)
{
    _aspect_ratio_is_locked = true;
    _aspect_ratio           = aspect_ratio;
    apply_aspect_ratio();
}

void ExportSize::apply_aspect_ratio()
{
    _size.set_width(static_cast<decltype(_size)::DataType>(
        _size.height() * _aspect_ratio.asFloat()
    ));
}

auto ExportSize::imgui() -> bool
{
    bool b = false;

    b |= ImageSizeU::imgui(_size);

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