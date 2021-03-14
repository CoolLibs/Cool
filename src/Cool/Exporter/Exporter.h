#pragma once

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
#include "internal/Averager.h"
#include <Cool/MultiThread/ThreadPool.h>
#endif

namespace Cool {

class FrameBuffer;

class Exporter {
public:
	Exporter();
	~Exporter() = default;

	/// <summary>
	/// Exports an image as PNG
	/// </summary>
	/// <param name="render">The function that renders the desired image</param>
	/// <param name="frame_buffer">The frame buffer that your render function has just rendered to</param>
	/// <param name="filepath">The name of the image file that you want to write</param>
	void export_image(std::function<void()> render, FrameBuffer& frameBuffer, const char* filepath);

	/// <summary>
	/// The buttons to open the different export windows.
	/// </summary>
	void ImGui_menu_items();

	/// <summary>
	/// The window with the image export parameters
	/// </summary>
	/// <param name="render">The function that renders the desired image</param>
	/// <param name="frame_buffer">The frame buffer that your render function has just rendered to</param>
	void ImGui_window_export_image(std::function<void()> render, FrameBuffer& frameBuffer);

	/// <summary>
	/// Opens or closes the window with the image export parameters.
	/// </summary>
	/// <param name="open">true iff the window should open, and false iff it should close</param>
	void open_window_export_image(bool open);

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
	/// <summary>
	/// Starts the export of the image sequence. You must then call update() on every frame after your rendering code.
	/// </summary>
	void begin_image_sequence_export();
	/// <summary>
	/// Call this after your rendering code. If we are exporting it will export the current frame and decide if the export should continue.
	/// </summary>
	/// <param name="frame_buffer">The frame buffer that your renderer has just rendered to.</param>
	void update(FrameBuffer& frame_buffer);
	/// <summary>
	/// Ends the export of the image sequence. It will be called automatically by update() once the end timestamp is reached.
	/// </summary>
	void end_image_sequence_export();
	/// <summary>
	/// The window with the image sequence export parameters
	/// </summary>
	void ImGui_window_export_image_sequence();
	/// <summary>
	/// Opens or closes the window with the image sequence export parameters.
	/// </summary>
	/// <param name="open">true iff the window should open, and false iff it should close</param>
	inline void open_window_export_image_sequence(bool open) { _is_window_open_image_sequence_export = open; }
#endif

private:
	std::string output_path();
	void find_available_file_name();
	void ImGui_resolution_widget();
#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
	void export_image_multithreaded(FrameBuffer& frame_buffer, const char* filepath);
#endif

private:
	std::string _folder_path_for_image;
	std::string _file_name = "img";
	bool _is_window_open_image_export = false;
	bool _should_show_file_exists_warning = false;

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
	ThreadPool _thread_pool{ 8 };
	std::string _folder_path_for_image_sequence;
	bool _is_exporting_image_sequence = false;
	float _fps = 30.f;
	float _sequence_begin_time_in_sec = 0.f;
	float _sequence_end_time_in_sec = 10.f;
	bool _is_window_open_image_sequence_export = false;
	int _frame_count;
	int _total_nb_of_frames_in_sequence;
	int _max_nb_digits_of_frame_count;
	Averager<float> _frame_time_average;
	std::chrono::steady_clock::time_point _last_export_date;
#endif
};

} // namespace Cool