#pragma once

#include <Cool/ImGui/ImGuiWindow.h>
#include <img/img.hpp>
#include "VideoExportParams.h"
#include "VideoExportProcess.h"

namespace Cool {

class Exporter {
public:
    Exporter();

    auto is_exporting() const -> bool { return _video_export_process.has_value(); }
    auto image_export_window() -> ImGuiWindow& { return _image_export_window; }
    auto video_export_window() -> ImGuiWindow& { return _video_export_window; }

    void set_aspect_ratio(float aspect_ratio);

    /// Displays all the currently active windows
    void imgui_windows(Polaroid polaroid, float time);

    /// The buttons to open the different export windows
    void imgui_menu_items(std::optional<float> aspect_ratio);

    /// Starts the export of the image sequence. You must then call update() on every frame after your rendering code
    void begin_video_export();

    /// Call this after your rendering code. If we are exporting it will export the current frame and decide if the export should continue
    void update(Polaroid polaroid);

    /// Ends the export of the image sequence. It will be called automatically by update() once the end timestamp is reached. You can also call it yourself to early exit of the export
    void end_video_export();

private:
    auto output_path() -> std::filesystem::path;
    void imgui_window_export_image(Polaroid polaroid, float time);
    void imgui_window_export_video();

private:
    img::Size _export_size{1920, 1080};

    std::filesystem::path _folder_path_for_image;
    std::filesystem::path _file_name{"img(0)"};
    ImGuiWindow           _image_export_window{"Export an Image", false};

    std::filesystem::path             _folder_path_for_video;
    ImGuiWindow                       _video_export_window{"Export a Video", false};
    std::optional<VideoExportProcess> _video_export_process;
    VideoExportParams                 _video_export_params;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Video Export Settings", _video_export_params),
            cereal::make_nvp("Image Size", _export_size),
            cereal::make_nvp("Image Output Folder", _folder_path_for_image),
            cereal::make_nvp("Video Output Folder", _folder_path_for_video)
        );
    }
};

} // namespace Cool