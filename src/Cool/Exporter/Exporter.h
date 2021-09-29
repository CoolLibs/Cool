#pragma once

#include <Cool/ImGui/ImGuiWindow.h>
#include <Cool/Image/ImageSize.h>
#include "VideoExportParams.h"
#include "VideoExportProcess.h"

namespace Cool {

class Exporter {
public:
    Exporter();

    bool is_exporting() const { return _video_export_process.has_value(); }

    ImGuiWindow& image_export_window() { return _image_export_window; }
    ImGuiWindow& video_export_window() { return _video_export_window; }

    /**
     * @brief Displays all the currently active windows
     * 
     */
    void imgui_windows(Polaroid polaroid);

    /**
	 * @brief The buttons to open the different export windows
	 * 
	 */
    void imgui_menu_items();

    /**
	 * @brief Starts the export of the image sequence. You must then call update() on every frame after your rendering code
	 * 
	 */
    void begin_image_sequence_export();

    /**
	 * @brief Call this after your rendering code. If we are exporting it will export the current frame and decide if the export should continue
	 * 
	 * @param frame_buffer The frame buffer that your renderer has just rendered to
	 */
    void update(Polaroid polaroid);

    /**
	 * @brief Ends the export of the image sequence. It will be called automatically by update() once the end timestamp is reached. You can also call it yourself to early exit of the export
	 * 
	 */
    void end_image_sequence_export();

private:
    std::string output_path();
    void        find_available_file_name();
    void        imgui_window_export_image(Polaroid polaroid);
    void        imgui_window_export_image_sequence();

private:
    ImageSize _export_size{1920, 1080};

    std::string _folder_path_for_image;
    std::string _file_name = "img(0)";
    ImGuiWindow _image_export_window{"Export an Image", false};
    bool        _should_show_file_exists_warning = false;

    std::string                       _folder_path_for_image_sequence;
    ImGuiWindow                       _video_export_window{"Export a Video", false};
    std::optional<VideoExportProcess> _video_export_process;
    VideoExportParams                 _video_export_params;
};

} // namespace Cool