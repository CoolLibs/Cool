#include "ImageExportJob.h"
#include <Cool/Image/SaveImage.h>

namespace Cool {

void ImageExportJob::operator()()
{
    const auto begin = std::chrono::steady_clock::now();
    ImageU::save_png(_file_path, *_image);
    const auto                   end        = std::chrono::steady_clock::now();
    std::chrono::duration<float> delta_time = end - begin;
    {
        std::unique_lock lock{*_export_average_time_mutex};
        _export_average_time->push(delta_time.count());
    }
    (*_nb_frames_which_finished_exporting)++;
}

} // namespace Cool
