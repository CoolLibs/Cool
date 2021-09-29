#include "ExporterU.h"
#include <Cool/ExportImage/as_png.h>
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>

namespace Cool::ExporterU {

void export_image(ImageSize size, ExporterInput in, std::string_view file_path)
{
    in.render(size);
    const auto image = in.render_target.download_pixels();
    if (File::create_folders_for_file_if_they_dont_exist(file_path)) {
        ExportImage::as_png(file_path, image.width(), image.height(), image.data.get());
    }
    else {
        Log::ToUser::warn("Exporter::export_image", "Failed to create folder \"{}\"", File::whithout_file_name(file_path));
    }
}

} // namespace Cool::ExporterU
