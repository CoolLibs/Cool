#pragma once

#include <Cool/ImGui/ImGuiWindow.h>
#include "ExportSize.h"
#include "VideoExportParams.h"
#include "VideoExportProcess.h"

namespace Cool {

/// Callbacks that will be called when the corresponding buttons are clicked.
/// If nothing is specified, the default behaviour is just to open the window.
/// If you specify your own callback, you have to ask for the window to open yourself, by calling
/// `exporter.image_export_window().open()` or `exporter.video_export_window().open()`.
struct imgui_menu_items_Params {
    std::optional<std::function<void()>> open_image_exporter = {};
    std::optional<std::function<void()>> open_video_exporter = {};
};

class Exporter {
public:
    Exporter();

    auto is_exporting() const -> bool { return _video_export_process.has_value(); }
    auto image_export_window() -> ImGuiWindow& { return _image_export_window; }
    auto video_export_window() -> ImGuiWindow& { return _video_export_window; }

    void set_aspect_ratio(AspectRatio aspect_ratio);
    void maybe_set_aspect_ratio(std::optional<AspectRatio>);

    /// Displays all the currently active windows
    void imgui_windows(Polaroid polaroid, float time);

    /// The buttons to open the different exporter windows.
    void imgui_menu_items(imgui_menu_items_Params = {});

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
    auto clear_export_folder() const -> bool;

private:
    ExportSize            _export_size;
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
        // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
        archive(
            cereal::make_nvp("Video Settings", _video_export_params),
            cereal::make_nvp("Image Size", _export_size),
            cereal::make_nvp("Image Output Folder", _folder_path_for_image),
            cereal::make_nvp("Video Output Folder", _folder_path_for_video)
        );
        // #else
        //         (void)archive;
        // #endif
    }
};

} // namespace Cool