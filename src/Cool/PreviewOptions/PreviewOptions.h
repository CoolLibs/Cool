#pragma once

#include <Cool/Gpu/InterpolationMode.h>
#include <Cool/Image/ImageSize.h>
#include <Cool/Utils/AspectRatio.h>

namespace Cool {

class PreviewOptions {
public:
    static PreviewOptions& instance()
    {
        static PreviewOptions render_state;
        return render_state;
    }

    void control_aspect_ratio(float aspect_ratio)
    {
        _aspect_ratio = aspect_ratio;
        should_control_aspect_ratio(true);
    }
    void control_nb_pixels(uint32_t nb_pixels)
    {
        _nb_pixels = nb_pixels;
        should_control_nb_pixels(true);
    }
    void should_control_aspect_ratio(bool should_control) { _is_controlling_aspect_ratio = should_control; }
    void should_control_nb_pixels(bool should_control) { _is_controlling_nb_pixels = should_control; }

    bool                     wants_to_constrain_size() const { return _is_controlling_aspect_ratio || _is_controlling_nb_pixels; }
    std::optional<ImageSize> constrained_size(ImageSize frame_size) const;
    ImageSize                preview_size(ImageSize frame_size) const;

    void imgui();

private:
    bool              _is_controlling_nb_pixels    = false;
    bool              _is_controlling_aspect_ratio = false;
    InterpolationMode _interpolation_mode          = InterpolationMode::NearestNeighbor;
    int               _nb_pixels                   = 250000;
    AspectRatio       _aspect_ratio;
};

} // namespace Cool