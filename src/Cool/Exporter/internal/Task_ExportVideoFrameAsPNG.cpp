#include "Cool/Image/SaveImage.h"
#include "Task_ExportVideoFrameAsPNG.hpp"

namespace Cool {

void Task_ExportVideoFrameAsPNG::do_work()
{
    auto const begin = std::chrono::steady_clock::now();

    if (!ImageU::save_png(_file_path, *_image, {.cancel = &_cancel}))
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
