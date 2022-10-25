#pragma once

#include <Cool/Utils/Averager.h>
#include <img/img.hpp>

namespace Cool {

class ImageExportJob {
public:
    ImageExportJob() = default;
    ImageExportJob(std::filesystem::path file_path, img::Image&& image, Averager<float>& average_export_time, std::mutex& average_export_time_mutex, std::atomic<int>& nb_frames_which_finished_exporting)
        : _file_path{file_path}
        , _image{std::move(image)}
        , _average_export_time{&average_export_time}
        , _average_export_time_mutex{&average_export_time_mutex}
        , _nb_frames_which_finished_exporting{&nb_frames_which_finished_exporting}
    {
    }
    ImageExportJob(ImageExportJob&& o) noexcept            = default;
    ImageExportJob& operator=(ImageExportJob&& o) noexcept = default;

    void operator()();

private:
    std::filesystem::path     _file_path;
    std::optional<img::Image> _image;
    Averager<float>*          _average_export_time;
    std::mutex*               _average_export_time_mutex;
    std::atomic<int>*         _nb_frames_which_finished_exporting;
};

} // namespace Cool
