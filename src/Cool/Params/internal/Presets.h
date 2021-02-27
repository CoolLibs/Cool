#pragma once

#include <Cool/File/File.h>
#include <Cool/String/String.h>
#include <Cool/ImGui/ImGui.h>
#include <Cool/Serialization/JsonFile.h>

namespace Cool {

template <typename T>
struct Preset {
	std::string name;
	T values;
};

template <typename T>
class Presets {
public:
	Presets(const std::string& file_extension, const std::string& folder_path)
		: _file_extension(file_extension + std::string(".")),
		  _folder_path(folder_path),
		  _save_preset_as(find_placeholder_name())
	{
		load_presets();
	}
	~Presets() = default;

	bool ImGui_dropdown(const char* label, T* setting_values) {
		bool b = false;
		if (ImGui::BeginCombo(label, _current_preset_name.c_str(), 0)) {
			for (size_t i = 0; i < _presets.size(); i++) {
				if (ImGui::Selectable(_presets[i].name.c_str(), false)) {
					b = true;
					_current_preset_name = _presets[i].name;
					_current_preset_idx = i;
					*setting_values = _presets[i].values;
				}
			}
			ImGui::EndCombo();
		}
		return b;
	}

	bool ImGui(T* setting_values) {
		bool b = ImGui_dropdown("Presets", setting_values);
		// Save preset
		if (!_current_preset_name.compare("Unsaved settings...")) {
			if (_name_available) {
				if (!_name_contains_dots) {
					if (ImGui::Button("Save settings")) {
						save_preset(*setting_values);
					}
					ImGui::SameLine();
					ImGui::Text("as");
				}
				else {
					ImGui::TextColored(ImVec4(0.74f, 0.04f, 0.04f, 1.f), "You can't have dots (.) in the name");
				}
			}
			else {
				ImGui::TextColored(ImVec4(0.74f, 0.04f, 0.04f, 1.f), "Name already used :");
			}
			ImGui::SameLine();
			ImGui::PushID(138571);
			if (ImGui::InputText("", &_save_preset_as)) {
				_name_available = !File::Exists(full_path(_save_preset_as)) && _save_preset_as.compare("Unsaved settings...");
				_name_contains_dots = _save_preset_as.find(".") != std::string::npos;
			}
			ImGui::PopID();
			_rename_popup_open_this_frame = false;
		}
		// Rename / Delete preset
		else {
			CoolImGui::InvisibleWrapperAroundPreviousLine("ovsidhsdh"); // Necessary otherwise we can't open a context menu on the dropdown
			if (ImGui::BeginPopupContextItem()) {
				if (CoolImGui::BeginPopupContextMenuFromButton("Rename")) {
					if (!_rename_popup_open_last_frame) {
						_new_preset_name = _current_preset_name;
					}
					ImGui::PushID(16879654123594);
					ImGui::InputText("", &_new_preset_name);
					ImGui::PopID();
					ImGui::EndPopup();
					_rename_popup_open_this_frame = true;
				}
				else {
					on_rename_popup_close();
				}
				if (ImGui::Button("Delete")) {
					if (boxer::show(("\"" + _current_preset_name + "\" will be deleted. Are you sure ?").c_str(), "Delete", boxer::Style::Warning, boxer::Buttons::YesNo) == boxer::Selection::Yes) {
						compute_current_preset_idx(); // Could have been changed by playing a record
						if (_current_preset_idx != -1) {
							std::filesystem::remove(full_path(_current_preset_name));
							_presets.erase(_current_preset_idx + _presets.begin());
						}
						else {
							boxer::show("Actually this preset isn't saved on your computer, it was just set by a record.\nYou will now be able to \"Save settings\" if you want to.", "Deleting failed", boxer::Style::Info);
						}
						find_placeholder_name();
						set_to_placeholder_setting();
					}
				}
			ImGui::EndPopup();
			}
			else {
				on_rename_popup_close();
			}
		}
		_rename_popup_open_last_frame = _rename_popup_open_this_frame;
		return b;
	}

	void set_to_placeholder_setting() {
		_current_preset_name = "Unsaved settings...";
		_current_preset_idx = -1;
	}

private:
	std::string full_path(const std::string& file_name) {
		return _folder_path + "/" + _file_extension + file_name + ".json";
	}
	void sort() {
		// Case insensitive alphabetical sort
		std::sort(_presets.begin(), _presets.end(), [](const Preset<T>& l, const Preset<T>& r) {
			return String::ToLower(l.name) < String::ToLower(r.name);
			});
		// Put Default first
		for (size_t i = 0; i < _presets.size(); ++i) {
			if (!_presets[i].name.compare("Default")) {
				Preset<T> prst = _presets[i];
				std::copy_backward(_presets.begin(), i + _presets.begin(), i + 1 + _presets.begin());
				_presets[0] = prst;
				break;
			}
		}
		//
		compute_current_preset_idx();
	}

	void on_rename_popup_close() {
		_rename_popup_open_this_frame = false;
		if (_rename_popup_open_last_frame) {
			compute_current_preset_idx(); // Could have been changed by playing a record
			if (_current_preset_idx != -1) {
				const std::string new_path = full_path(_new_preset_name);
				if (!File::Exists(new_path)) {
					std::filesystem::rename(
						full_path(_current_preset_name),
						new_path
					);
					_current_preset_name = _new_preset_name;
					_presets[_current_preset_idx].name = _new_preset_name;
					sort();
				}
				else {
					boxer::show(("\"" + _new_preset_name + "\" already exists.").c_str(), "Renaming failed", boxer::Style::Warning);
				}
			}
			else {
				boxer::show("Actually this preset isn't saved on your computer, it was just set by a record.\nYou will now be able to \"Save settings\" if you want to.", "Renaming failed", boxer::Style::Info);
				find_placeholder_name();
				set_to_placeholder_setting();
			}
		}
	}

	void compute_current_preset_idx() {
		for (size_t i = 0; i < _presets.size(); ++i) {
			if (!_presets[i].name.compare(_current_preset_name)) {
				_current_preset_idx = i;
				break;
			}
		}
	}

	std::string find_placeholder_name() {
		int i = 1;
		std::string name = "MyPreset";
		while (File::Exists(full_path(name))) {
			name = "MyPreset(" + std::to_string(i) + ")";
			i++;
		}
		return name;
	}

	void load_presets() { 
		if (File::Exists(_folder_path)) {
			for (const auto& file : std::filesystem::directory_iterator(_folder_path)) {
				if (!file.path().filename().replace_extension("").replace_extension(".").string().compare(_file_extension)) {
					std::string name = file.path().filename().replace_extension("").extension().string().erase(0, 1);
					T values;
					std::ifstream is(file.path());
					try {
						cereal::JSONInputArchive archive(is);
						archive(
							values
						);
						_presets.push_back({ name, values });
					}
					catch (std::exception e) {
						Log::Release::Warn("Invalid file {}.\n{}", file.path().string(), e.what());
					}
				}
			}
			sort();
		}
	}

	void save_preset(T& settingValues) {
		Serialization::ToJSON(settingValues, full_path(_save_preset_as));
		// Add it to current list
		_presets.push_back({ _save_preset_as, settingValues });
		// Give the name to the selected preset
		_current_preset_name = _save_preset_as;
		//
		sort();
		// Find new placeholder name
		_save_preset_as = find_placeholder_name();
	}

private:
	const std::string _file_extension;
	const std::string _folder_path; // Must be declared before _save_preset_as
	std::string _current_preset_name = "Unsaved settings...";
	size_t _current_preset_idx = -1;
	std::vector<Preset<T>> _presets;
	std::string _save_preset_as;
	bool _name_available = true;
	bool _name_contains_dots = false;
	std::string _new_preset_name;
	bool _rename_popup_open_last_frame = false;
	bool _rename_popup_open_this_frame = false;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void save(Archive& archive, std::uint32_t const version) const
	{
		archive(
			CEREAL_NVP(_current_preset_name)
		);
	}
	template <class Archive>
	void load(Archive& archive, std::uint32_t const version)
	{
		archive(
			CEREAL_NVP(_current_preset_name)
		);
		compute_current_preset_idx();
	}
};

} // namespace Cool