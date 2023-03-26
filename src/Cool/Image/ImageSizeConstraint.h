#pragma once

#include <Cool/Gpu/InterpolationMode.h>
#include <img/img.hpp>
#include "AspectRatio.h"

namespace Cool {

class ImageSizeConstraint {
public:
    void take_control_of_aspect_ratio(float aspect_ratio)
    {
        _aspect_ratio = aspect_ratio;
        should_control_aspect_ratio(true);
    }
    void take_control_of_nb_pixels(uint32_t nb_pixels)
    {
        _nb_pixels = nb_pixels;
        should_control_nb_pixels(true);
    }
    void should_control_aspect_ratio(bool should_control) { _is_controlling_aspect_ratio = should_control; }
    void should_control_nb_pixels(bool should_control) { _is_controlling_nb_pixels = should_control; }

    bool      wants_to_constrain_size() const { return _is_controlling_aspect_ratio || _is_controlling_nb_pixels; }
    bool      wants_to_constrain_aspect_ratio() const { return _is_controlling_aspect_ratio; }
    img::Size applied_to(img::Size frame_size) const;
    auto      aspect_ratio() const -> std::optional<AspectRatio> { return _is_controlling_aspect_ratio
                                                                              ? std::make_optional(_aspect_ratio)
                                                                              : std::nullopt; }

    bool imgui();

private:
    img::Size compute_constraints_on(img::Size frame_size) const;

private:
    bool              _is_controlling_nb_pixels    = false;
    bool              _is_controlling_aspect_ratio = false;
    InterpolationMode _interpolation_mode          = InterpolationMode::NearestNeighbor;
    uint32_t          _nb_pixels                   = 250'000;
    AspectRatio       _aspect_ratio;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
        archive(
            cereal::make_nvp("Is controlling the number of pixels", _is_controlling_nb_pixels),
            cereal::make_nvp("Number of pixels", _nb_pixels),
            cereal::make_nvp("Is controlling the aspect ratio", _is_controlling_aspect_ratio),
            cereal::make_nvp("Aspect ratio", _aspect_ratio),
            cereal::make_nvp("Interpolation mode", _interpolation_mode)
        );
        // #else
        //         (void)archive;
        // #endif
    }
};

} // namespace Cool