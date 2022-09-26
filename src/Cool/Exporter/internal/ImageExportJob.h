#pragma once

#include <Cool/Utils/Averager.h>
#include <img/img.hpp>

namespace Cool {

class ImageExportJob {
public:
    ImageExportJob() = default;
    ImageExportJob(std::filesystem::path file_path, img::Image&& image, Averager<float>* frame_time_average, std::atomic<int>* nb_frames_which_finished_exporting)
        : _file_path{file_path}
        , _image{std::move(image)}
        , _frame_time_average{frame_time_average}
        , _nb_frames_which_finished_exporting{nb_frames_which_finished_exporting}
    {
    }
    ImageExportJob(ImageExportJob&& o) noexcept            = default;
    ImageExportJob& operator=(ImageExportJob&& o) noexcept = default;

    void operator()();

private:
    std::filesystem::path     _file_path;
    std::optional<img::Image> _image;
    Averager<float>*          _frame_time_average;
    std::atomic<int>*         _nb_frames_which_finished_exporting; // TODO use a reference_wrapper
};

} // namespace Cool
