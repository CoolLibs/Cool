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
    void imgui(std::function<void()> const& extra_widgets);

    auto clock() const -> Clock const& { return _clock; }

private:
    auto estimated_remaining_time() -> float;
    void update_time_estimate();
    void export_frame(Polaroid polaroid, std::filesystem::path const& file_path);

private:
    std::filesystem::path _folder_path;
    img::Size             _size;
    Clock_FixedTimestep   _clock;

    int              _nb_frames_sent_to_thread_pool{0};
    std::atomic<int> _nb_frames_which_finished_exporting{0};
    int              _total_nb_of_frames_in_sequence;
    int              _frame_numbering_offset;

    std::chrono::steady_clock::time_point _last_render{};
    Averager<float>                       _average_time_between_two_renders{};
    Averager<float>                       _average_export_time{};
    std::mutex                            _average_export_time_mutex;

    bool                       _should_stop_asap = false;
    ThreadPool<ImageExportJob> _thread_pool; // Needs to be last, in order to be destroyed first (because it needs the other members to still be alive in order to finish its jobs properly)
};

} // namespace Cool
