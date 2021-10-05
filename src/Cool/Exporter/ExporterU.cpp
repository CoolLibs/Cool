#include "ExporterU.h"
#include <Cool/File/File.h>
#include <Cool/Image/ExportImage.h>
#include <Cool/Log/ToUser.h>

namespace Cool::ExporterU {

void export_image(ImageSize size, float time, Polaroid polaroid, std::string_view file_path)
{
    polaroid.render(time, size);
    ExportImage::as_png(file_path, polaroid.render_target.download_pixels());
}

} // namespace Cool::ExporterU
