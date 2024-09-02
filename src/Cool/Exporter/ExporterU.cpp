#include "ExporterU.h"
#include <Cool/File/File.h>
#include <Cool/Image/SaveImage.h>
#include <Cool/Log/ToUser.h>

namespace Cool::ExporterU {

void export_image(img::Size size, Time time, Time delta_time, Polaroid const& polaroid, std::filesystem::path const& file_path)
{
    polaroid.render(size, time, delta_time);
    ImageU::save_png(file_path, polaroid.texture().download_pixels());
}

} // namespace Cool::ExporterU
