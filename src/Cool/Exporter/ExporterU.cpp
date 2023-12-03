#include "ExporterU.h"
#include <Cool/File/File.h>
#include <Cool/Image/SaveImage.h>
#include <Cool/Log/ToUser.h>

namespace Cool::ExporterU {

void export_image(img::Size size, float time, float delta_time, Polaroid polaroid, std::filesystem::path file_path)
{
    polaroid.render(time, delta_time, size);
    ImageU::save_png(file_path, polaroid.render_target.download_pixels());
}

} // namespace Cool::ExporterU
