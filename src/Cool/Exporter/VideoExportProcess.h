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
    void export_frame(Polaroid polaroid, std::filesystem::path file_path);

private:
    ThreadPool<ImageExportJob> _thread_pool;
    int                        _nb_frames_sent_to_thread_pool{0};
    std::atomic<int>           _nb_frames_which_finished_exporting{0};
    int                        _total_nb_of_frames_in_sequence;
    int                        _max_nb_digits_of_frame_count;
    int                        _frame_numbering_offset;
    Averager<float>            _frame_time_average;
    std::filesystem::path      _folder_path;
    bool                       _should_stop_asap = false;
    img::Size                  _size;
    Clock_FixedTimestep        _clock;
};

} // namespace Cool