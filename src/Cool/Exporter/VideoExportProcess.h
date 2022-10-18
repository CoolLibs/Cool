#pragma once

#include <Cool/MultiThread/ThreadPool.h>
#include <Cool/Time/Clock_FixedTimestep.h>
#include <Cool/Utils/Averager.h>
#include "VideoExportParams.h"
#include "internal/ImageExportJob.h"
#include "internal/Polaroid.h"

namespace Cool {

class VideoExportProcess {
public:
    VideoExportProcess(const VideoExportParams& params, std::filesystem::path folder_path, img::Size size);
    auto update(Polaroid polaroid) -> bool;
    void imgui();

private:
    auto estimated_remaining_time() -> float;
    void export_frame(Polaroid polaroid, std::filesystem::path file_path);

private:
    ThreadPool<ImageExportJob>            _thread_pool;
    int                                   _nb_frames_sent_to_thread_pool{0};
    std::atomic<int>                      _nb_frames_which_finished_exporting{0};
    int                                   _total_nb_of_frames_in_sequence;
    int                                   _frame_numbering_offset;
    Averager<float>                       _export_average_time;
    std::mutex                            _export_average_time_mutex;
    Averager<float>                       _rendering_average_time;
    Averager<float>                       _time_between_two_exports;
    std::chrono::steady_clock::time_point _last_export;
    std::filesystem::path                 _folder_path;
    bool                                  _should_stop_asap = false;
    img::Size                             _size;
    Clock_FixedTimestep                   _clock;
    float                                 _last_dt = 0.f;
};

} // namespace Cool