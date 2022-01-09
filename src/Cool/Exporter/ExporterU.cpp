#include "ExporterU.h"
#include <Cool/File/File.h>
#include <Cool/Image/SaveImage.h>
#include <Cool/Log/ToUser.h>

namespace Cool::ExporterU {

void export_image(img::Size size, float time, Polaroid polaroid, std::string_view file_path)
{
    polaroid.render(time, size);
    ImageU::save_png(file_path, polaroid.render_target.download_pixels());
}

} // namespace Cool::ExporterU
