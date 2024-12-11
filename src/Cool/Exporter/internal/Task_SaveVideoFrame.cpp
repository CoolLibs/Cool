#include "Task_SaveVideoFrame.hpp"
#include "../ExporterU.h"
#include "Cool/Image/SaveImage.h"
#include "ImGuiNotify/ImGuiNotify.hpp"

namespace Cool {

void Task_SaveVideoFrame::do_work()
{
    auto const begin = std::chrono::steady_clock::now();

    auto const result = ImageU::save(_file_path, *_image, {.cancel = &_cancel});
    if (!result.has_value())
    {
        if (_cancel.load())
            return; // Don't log any error if we were just canceled
        bool const has_already_been_reported = _report_failure->exchange(true);
        if (!has_already_been_reported)
            ImGuiNotify::send(ExporterU::notification_after_export_failure(result.error(), true));
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
