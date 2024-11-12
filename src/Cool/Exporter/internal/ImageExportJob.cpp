#include "ImageExportJob.h"
#include <Cool/Image/SaveImage.h>

namespace Cool {

void ImageExportJob::operator()()
{
    auto const begin = std::chrono::steady_clock::now();

    if (!ImageU::save_png(_file_path, *_image))
    {
        *_report_failure = true;
        return;
    }

    auto const end        = std::chrono::steady_clock::now();
    auto const delta_time = std::chrono::duration<float>{end - begin};
    {
        auto lock = std::unique_lock{*_average_export_time_mutex};
        _average_export_time->push(delta_time.count());
    }
    (*_nb_frames_which_finished_exporting)++;
}

} // namespace Cool
