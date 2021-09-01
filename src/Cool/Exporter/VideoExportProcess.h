#pragma once

#include <Cool/MultiThread/ThreadPool.h>
#include <Cool/Utils/Averager.h>
#include "VideoExportParams.h"
#include "internal/ExporterInput.h"
#include "internal/ImageExportJob.h"

namespace Cool {

class VideoExportProcess {
public:
    VideoExportProcess(const VideoExportParams& params, std::string_view folder_path_for_image_sequence, ImageSize size);
    ~VideoExportProcess();
    bool update(ExporterInput input);
    void imgui();

private:
    void export_frame(ExporterInput in, std::string_view file_path);

private:
    VideoExportParams          _params;
    ThreadPool<ImageExportJob> _thread_pool;
    int                        _nb_frames_sent_to_thread_pool;
    std::atomic<int>           _nb_frames_which_finished_exporting;
    int                        _total_nb_of_frames_in_sequence;
    int                        _max_nb_digits_of_frame_count;
    Averager<float>            _frame_time_average;
    std::string                _folder_path_for_image_sequence;
    bool                       _should_stop_asap = false;
    ImageSize                  _size;
};

} // namespace Cool