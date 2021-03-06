#include "ImageExportJob.h"
#include <Cool/Image/SaveImage.h>

namespace Cool {

void ImageExportJob::operator()()
{
    auto begin = std::chrono::steady_clock::now();
    ImageU::save_png(_file_path, *_image);
    auto                         end        = std::chrono::steady_clock::now();
    std::chrono::duration<float> delta_time = end - begin;
    _frame_time_average->push(delta_time.count());
    (*_nb_frames_which_finished_exporting)++;
}

} // namespace Cool
