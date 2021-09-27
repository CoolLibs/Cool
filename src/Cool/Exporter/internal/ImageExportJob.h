#pragma once

#include <Cool/Image/ImageData.h>
#include <Cool/Utils/Averager.h>

namespace Cool {

class ImageExportJob {
public:
    ImageExportJob() = default;
    ImageExportJob(std::string_view file_path, ImageData&& image, Averager<float>* frame_time_average, std::atomic<int>* nb_frames_which_finished_exporting)
        : _file_path(file_path), _image{std::move(image)}, _frame_time_average(frame_time_average), _nb_frames_which_finished_exporting(nb_frames_which_finished_exporting)
    {
    }
    ImageExportJob(ImageExportJob&& o) noexcept = default;
    ImageExportJob& operator=(ImageExportJob&& o) noexcept = default;

    void operator()();

private:
    std::string       _file_path;
    ImageData         _image;
    Averager<float>*  _frame_time_average;
    std::atomic<int>* _nb_frames_which_finished_exporting;
};

} // namespace Cool
