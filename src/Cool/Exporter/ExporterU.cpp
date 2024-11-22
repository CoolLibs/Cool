#include "ExporterU.h"
#include <Cool/File/File.h>
#include <Cool/Image/SaveImage.h>
#include <Cool/Log/ToUser.h>

namespace Cool::ExporterU {

auto export_image(img::Size size, Time time, Time delta_time, Polaroid const& polaroid, std::filesystem::path const& file_path) -> bool
{
    no_sleep::Scoped disable_sleep{COOL_APP_NAME, COOL_APP_NAME " is exporting an image", no_sleep::Mode::ScreenCanTurnOffButKeepComputing};
    polaroid.render(size, time, delta_time);
    return ImageU::save_png(file_path, polaroid.texture().download_pixels());
}

} // namespace Cool::ExporterU
