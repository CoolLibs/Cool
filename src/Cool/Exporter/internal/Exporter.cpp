#include "../Exporter.h"

#include <Cool/App/RenderState.h>
#include <Cool/OpenGL/FrameBuffer.h>
#include <Cool/File/File.h>
#include <Cool/ExportImage/AsPNG.h>
#include <Cool/ImGuiExtras/ImGuiExtras.h>
#include <Cool/Time/Time.h>
#include <Cool/String/String.h>
#include <chrono>

namespace Cool {

class ExportImage_Functor {
public:
	ExportImage_Functor(std::string_view filepath, int width, int height, std::vector<unsigned char>&& data, Averager<float>& frame_time_average, std::atomic<int>& nb_frames_which_finished_exporting)
		: _filepath(filepath), _width(width), _height(height), _data(data), _frame_time_average(frame_time_average), _nb_frames_which_finished_exporting(nb_frames_which_finished_exporting)
	{}
	ExportImage_Functor(ExportImage_Functor&& o) noexcept
		: _filepath(std::move(o._filepath)), _width(o._width), _height(o._height), _data(std::move(o._data)), _frame_time_average(o._frame_time_average), _nb_frames_which_finished_exporting(o._nb_frames_which_finished_exporting)
	{}
	ExportImage_Functor(const ExportImage_Functor& o) noexcept
		: _filepath(o._filepath), _width(o._width), _height(o._height), _data(o._data), _frame_time_average(o._frame_time_average), _nb_frames_which_finished_exporting(o._nb_frames_which_finished_exporting)
	{}

	void operator()() {
		auto begin = std::chrono::steady_clock::now();
		ExportImage::AsPNG(_filepath, _width, _height, _data.data());
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<float> delta_time = end - begin;
		_frame_time_average.push(delta_time.count());
		_nb_frames_which_finished_exporting++;
	}

private:
	std::string _filepath;
	int _width;
	int _height;
	std::vector<unsigned char> _data;
	Averager<float>& _frame_time_average;
	std::atomic<int>& _nb_frames_which_finished_exporting;
};

Exporter::Exporter()
	: _folder_path_for_image(File::root_dir() + "/out")
	, _folder_path_for_image_sequence(File::root_dir() + "/exports")
{}

void Exporter::export_image(std::function<void()> render, FrameBuffer& frame_buffer, std::string_view filepath) {
	// Render
	RenderState::setIsExporting(true);
	render();
	// Get data
	frame_buffer.bind();
	auto size = RenderState::Size();
	std::vector<unsigned char> data(4 * size.width() * size.height());
	glReadPixels(0, 0, size.width(), size.height(), GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	frame_buffer.unbind();
	// Write png
	if (File::CreateFoldersIfDoesntExist(_folder_path_for_image.c_str())) {
		ExportImage::AsPNG(filepath, size.width(), size.height(), data.data());
	}
	else {
		Log::warn("[Exporter::export_image] Failed to create folder '{}'", _folder_path_for_image);
	}
	//
	RenderState::setIsExporting(false);
}

void Exporter::export_image_multithreaded(FrameBuffer& frame_buffer, std::string_view filepath) {
	// Wait for a thread to be available
	_thread_pool.wait_for_available_thread();
	// Get data
	frame_buffer.bind();
	auto size = RenderState::Size();
	std::vector<unsigned char> data(4 * size.width() * size.height());
	glReadPixels(0, 0, size.width(), size.height(), GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	frame_buffer.unbind();
	// Write png
	_thread_pool.push_job(std::move(ExportImage_Functor(filepath, size.width(), size.height(), std::move(data), _frame_time_average, _nb_frames_which_finished_exporting)));
}

std::string Exporter::output_path() {
	return _folder_path_for_image + "/" + _file_name + ".png";
}

void Exporter::find_available_file_name() {
	if (File::Exists(output_path().c_str())) {
		// Find base_name and k
		int k = 1;
		std::string base_name = _file_name;
		size_t pos = _file_name.find_last_of("(");
		if (pos != std::string::npos) {
			// Find number in parenthesis
			base_name = _file_name.substr(0, pos);
			size_t end_pos = _file_name.find_last_of(")");
			try {
				k = std::stoi(_file_name.substr(pos + 1, end_pos - pos));
			}
			catch (std::exception e) {
				k = 1;
				base_name = _file_name;
			}
		}
		// Find available name
		while (File::Exists(output_path().c_str())) {
			_file_name = base_name + "(" + std::to_string(k) + ")";
			k++;
		}
	}
}

void Exporter::open_window_export_image(bool open) {
	_is_window_open_image_export = open;
	if (_is_window_open_image_export)
		find_available_file_name();
}

void Exporter::ImGui_menu_items() {
	// Calculate max button width
	const char* longuest_text = "Image Sequence";
	float button_width = ImGui::CalcTextSize(longuest_text).x + 2.f * ImGui::GetStyle().FramePadding.x;
	// Draw buttons
	//ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
	if (ImGui::Button("Image", ImVec2(button_width, 0.0f))) {
		open_window_export_image(true);
	}
	if (ImGui::Button("Image Sequence", ImVec2(button_width, 0.0f))) {
		_is_window_open_image_sequence_export = true;
	}
	//ImGui::PopStyleVar();
}

void Exporter::ImGui_resolution_widget() {
	bool _was_used = false;
	ImGui::Text("Resolution : "); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	unsigned int w = static_cast<unsigned int>(RenderState::getExportSize().x);
	unsigned int h = static_cast<unsigned int>(RenderState::getExportSize().y);
	_was_used |= ImGui::InputUInt("W", &w); ImGui::SameLine();
	_was_used |= ImGui::InputUInt("H", &h);
	ImGui::PopItemWidth();
	if (_was_used)
		RenderState::setExportSize(static_cast<int>(w), static_cast<int>(h));
}

void Exporter::ImGui_window_export_image(std::function<void()> render, FrameBuffer& frameBuffer) {
	if (_is_window_open_image_export) {
		ImGui::Begin("Export an Image", &_is_window_open_image_export);
		// Resolution
		ImGui_resolution_widget();
		// File and Folders
		bool _path_has_changed = false;
		_path_has_changed |= ImGui::InputText("File Name", &_file_name);
		_path_has_changed |= ImGui::InputText("Path", &_folder_path_for_image);
		ImGui::SameLine();
		_path_has_changed |= ImGui::OpenFolderDialog(&_folder_path_for_image, _folder_path_for_image);
		if (_path_has_changed) {
			_should_show_file_exists_warning = File::Exists(output_path().c_str());
		}
		// Warning file exists
		ImGui::NewLine();
		if (_should_show_file_exists_warning) {
			ImGui::WarningText("This file already exists. Are you sure you want to overwrite it ?");
		}
		// Validation
		if (ImGui::Button("Export as PNG")) {
			_is_window_open_image_export = false;
			export_image(render, frameBuffer, output_path().c_str());
		}
		//
		ImGui::End();
	}
}

void Exporter::begin_image_sequence_export() {
	if (File::CreateFoldersIfDoesntExist(_folder_path_for_image_sequence.c_str())) {
		_thread_pool.start();
		_is_exporting_image_sequence = true;
		RenderState::setIsExporting(true);
		_nb_frames_sent_to_thread_pool = 0;
		_nb_frames_which_finished_exporting = 0;
		float total_export_duration = _sequence_end_time_in_sec - _sequence_begin_time_in_sec;
		_total_nb_of_frames_in_sequence = static_cast<unsigned int>(std::ceil(total_export_duration * _fps));
		_max_nb_digits_of_frame_count = static_cast<int>(std::ceil(std::log10(_total_nb_of_frames_in_sequence)));
		Time::set_elapse_mode_as_fixed_timestep(_fps);
		Time::set_time(_sequence_begin_time_in_sec);
		_frame_time_average.clear();
	}
	else {
		Log::ToUser::warn("[Exporter::begin_image_sequence_export] Couldn't start exporting because folder creation failed !");
	}
}

void Exporter::update(FrameBuffer& frameBuffer) {
	if (_is_exporting_image_sequence) {
		_is_window_open_image_sequence_export = true;
		if (_nb_frames_which_finished_exporting.load() < _total_nb_of_frames_in_sequence) {
			if (_nb_frames_sent_to_thread_pool < _total_nb_of_frames_in_sequence) {
				export_image_multithreaded(frameBuffer, (_folder_path_for_image_sequence + "/" + String::to_string(_nb_frames_sent_to_thread_pool, _max_nb_digits_of_frame_count) + ".png").c_str());
				_nb_frames_sent_to_thread_pool++;
			}
		}
		else {
			end_image_sequence_export();
		}
	}
}

void Exporter::end_image_sequence_export() {
	_thread_pool.stop();
	_is_exporting_image_sequence = false;
	RenderState::setIsExporting(false);
	Time::set_elapse_mode_as_realtime();
	_is_window_open_image_sequence_export = false;
}

bool Exporter::ImGui_window_export_image_sequence() {
	bool has_started = false;
	if (_is_window_open_image_sequence_export) {
		ImGui::Begin("Export an Image Sequence", _is_exporting_image_sequence ? nullptr : &_is_window_open_image_sequence_export);
		// Not exporting
		if (!_is_exporting_image_sequence) {
			ImGui_resolution_widget();
			ImGui::InputText("Path", &_folder_path_for_image_sequence);
			ImGui::SameLine();
			ImGui::OpenFolderDialog(&_folder_path_for_image_sequence, _folder_path_for_image_sequence);
			ImGui::InputFloat("FPS", &_fps);
			ImGui::PushItemWidth(50);
			ImGui::Text("From"); ImGui::SameLine(); ImGui::PushID(13540);
			ImGui::DragFloat("", &_sequence_begin_time_in_sec); ImGui::PopID(); ImGui::SameLine();
			ImGui::Text("To"); ImGui::SameLine(); ImGui::PushID(14540);
			ImGui::DragFloat("", &_sequence_end_time_in_sec); ImGui::PopID(); ImGui::SameLine();
			ImGui::Text("seconds");
			ImGui::PopItemWidth();
			// Validation
			if (ImGui::Button("Start exporting")) {
				_is_exporting_image_sequence = true;
				has_started = true;
				begin_image_sequence_export();
			}
		}
		// Exporting
		else {
			int frame_count = _nb_frames_which_finished_exporting.load();
			ImGui::Text(("Exported " + String::to_string(frame_count, _max_nb_digits_of_frame_count) + " / " + std::to_string(_total_nb_of_frames_in_sequence) + " frames").c_str());
			ImGui::TimeFormatedHMS((_total_nb_of_frames_in_sequence - frame_count) * _frame_time_average / _thread_pool.size()); ImGui::SameLine();
			ImGui::Text("remaining");
			if (ImGui::Button("Stop exporting")) {
				end_image_sequence_export();
			}
		}
		ImGui::End();
	}
	return has_started;
}

} // namespace Cool