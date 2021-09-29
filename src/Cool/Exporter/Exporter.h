#pragma once

#include <Cool/Image/ImageSize.h>
#include "VideoExportParams.h"
#include "VideoExportProcess.h"

namespace Cool {

class Exporter {
public:
    Exporter();

    /**
	 * @brief The buttons to open the different export windows
	 * 
	 */
    void imgui_menu_items();

    /**
	 * @brief The window with the image export parameters
	 * 
	 * @param render The function that renders the desired image
	 * @param frame_buffer The frame buffer that your render function will render to
	 */
    void imgui_window_export_image(ExporterInput input);

    /**
	 * @brief Opens or closes the window with the image export parameters
	 * 
	 * @param open true iff the window should open, and false iff it should close
	 */
    void open_window_export_image(bool open = true);

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
    void update(ExporterInput input);

    /**
	 * @brief Ends the export of the image sequence. It will be called automatically by update() once the end timestamp is reached. You can also call it yourself to early exit of the export
	 * 
	 */
    void end_image_sequence_export();

    /**
	 * @brief The window with the image sequence export parameters
	 * 
	 * @return true iff the export has just started
	 */
    bool imgui_window_export_image_sequence();

    bool is_exporting() const { return _video_export_process.has_value(); }

    /**
	 * @brief Opens or closes the window with the image sequence export parameters
	 * 
	 * @param open true iff the window should open, and false iff it should close
	 */
    inline void open_window_export_image_sequence(bool open = true) { _is_window_open_image_sequence_export = open; }

private:
    std::string output_path();
    void        find_available_file_name();

private:
    ImageSize _export_size{1920, 1080};

    std::string _folder_path_for_image;
    std::string _file_name                       = "img(0)";
    bool        _is_window_open_image_export     = false;
    bool        _should_show_file_exists_warning = false;

    std::string                       _folder_path_for_image_sequence;
    bool                              _is_window_open_image_sequence_export = false;
    std::optional<VideoExportProcess> _video_export_process;
    VideoExportParams                 _video_export_params;
};

} // namespace Cool