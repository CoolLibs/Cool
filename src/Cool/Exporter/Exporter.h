#pragma once

#include <Cool/MultiThread/ThreadPool.h>
#include <Cool/Rendering/IRenderTarget.h>
#include <Cool/Utils/Averager.h>

namespace Cool {

class Exporter {
public:
    Exporter();

    /**
	 * @brief Exports an image as PNG
	 * 
	 * @param render The function that renders the desired image
	 * @param frame_buffer The frame buffer that your render function will render to
	 * @param file_path The name of the image file that you want to write
	 */
    void export_image(std::function<void()> render, const IRenderTarget& render_target, std::string_view file_path);

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
    void imgui_window_export_image(std::function<void()> render, const IRenderTarget& render_target);

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
    void update(const IRenderTarget& render_target);

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

    /**
	 * @brief Opens or closes the window with the image sequence export parameters
	 * 
	 * @param open true iff the window should open, and false iff it should close
	 */
    inline void open_window_export_image_sequence(bool open = true) { _is_window_open_image_sequence_export = open; }

private:
    std::string output_path();
    void        find_available_file_name();
    void        imgui_resolution_widget();
    void        export_image_multithreaded(const IRenderTarget& render_target, std::string_view file_path);

private:
    std::string _folder_path_for_image;
    std::string _file_name                       = "img(0)";
    bool        _is_window_open_image_export     = false;
    bool        _should_show_file_exists_warning = false;

    ThreadPool       _thread_pool;
    std::string      _folder_path_for_image_sequence;
    bool             _is_exporting_image_sequence          = false;
    float            _fps                                  = 30.f;
    float            _sequence_begin_time_in_sec           = 0.f;
    float            _sequence_end_time_in_sec             = 10.f;
    bool             _is_window_open_image_sequence_export = false;
    int              _nb_frames_sent_to_thread_pool;
    std::atomic<int> _nb_frames_which_finished_exporting;
    int              _total_nb_of_frames_in_sequence;
    int              _max_nb_digits_of_frame_count;
    Averager<float>  _frame_time_average;
};

} // namespace Cool