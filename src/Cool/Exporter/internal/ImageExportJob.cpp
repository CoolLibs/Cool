#include "ImageExportJob.h"
#include <Cool/Image/SaveImage.h>

namespace Cool {

void ImageExportJob::operator()()
{
    const auto begin = std::chrono::steady_clock::now();
    ImageU::save_png(_file_path, *_image);
    const auto end        = std::chrono::steady_clock::now();
    const auto delta_time = std::chrono::duration<float>{end - begin};
    {
        std::unique_lock lock{*_average_export_time_mutex};
        _average_export_time->push(delta_time.count());
    }
    (*_nb_frames_which_finished_exporting)++;
}

} // namespace Cool
