#include "ImageSizeConstraint.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include "ImageSizeU.h"
#include "compute_image_size.h"

namespace Cool {

auto ImageSizeConstraint::applied_to(img::Size frame_size) const -> img::Size
{
    if (wants_to_constrain_size())
    {
        return compute_constraints_on(frame_size);
    }
    else
    {
        return frame_size;
    }
}

auto ImageSizeConstraint::compute_constraints_on(img::Size frame_size) const -> img::Size
{
    float const aspect_ratio = _is_controlling_aspect_ratio
                                   ? _aspect_ratio.get()
                                   : img::SizeU::aspect_ratio(frame_size);

    auto const nb_pixels = static_cast<float>(
        _is_controlling_nb_pixels
            ? _nb_pixels
            : frame_size.width() * frame_size.height()
    );

    return compute_image_size(aspect_ratio, nb_pixels);
}

auto ImageSizeConstraint::imgui() -> bool
{
    bool was_triggered = false;
    // Aspect Ratio
    was_triggered |= ImGuiExtras::checkbox_with_submenu("Fixed aspect ratio", &_is_controlling_aspect_ratio, [&]() {
        return _aspect_ratio.imgui();
    });
    // Nb Pixels
    was_triggered |= ImGuiExtras::checkbox_with_submenu("Fixed number of pixels", &_is_controlling_nb_pixels, [&]() {
        auto previewNbPixels = _nb_pixels;
        if (ImGuiExtras::slider_uint32("Number of Pixels", &previewNbPixels, 10'000, 500'000))
        {
            _nb_pixels = previewNbPixels;
            return true;
        }
        return false;
    });
    // Interpolation mode // Disabled for now as this would require a bit of work to apply it to the current view + I don't find this very useful, Nearest is always what you want imho.
    // was_triggered |= ImGui::Combo("Interpolation Mode", reinterpret_cast<int*>(&_interpolation_mode), "Nearest\0Linear\0\0");

    return was_triggered;
}

} // namespace Cool