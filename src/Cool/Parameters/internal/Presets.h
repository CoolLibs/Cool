#pragma once

#include <Cool/File/File.h>
#include <Cool/String/String.h>
#include <Cool/ImGuiExtras/ImGuiExtras.h>
#include <Cool/Serialization/JsonFile.h>
#include <Cool/Random/Random.h>

namespace Cool {

template <typename T>
struct ValuesWithUUID {
	T values;
	long int uuid;

	ValuesWithUUID()
	{}

	ValuesWithUUID(const T& values, long int uuid)
		: values(values), uuid(uuid)
	{}

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp("Values", values),
			cereal::make_nvp("uuid", uuid)
		);
	}
};

template <typename T>
struct Preset {
	std::string name;
	ValuesWithUUID<T> values_with_uuid;

	Preset(const std::string& name, const T& values)
		: name(name), values_with_uuid(values, Random::long_int())
	{}

	Preset(const std::string& name, const T& values, long int uuid)
		: name(name), values_with_uuid(values, uuid)
	{}
};

template <typename T>
class Presets {
	template <typename T>
	friend class ParametersWithPresets;
public:
	Presets(const std::string& file_extension, const std::string& folder_path)
		: _file_extension(file_extension + std::string("."))
		, _folder_path(folder_path)
		, _save_preset_as(find_placeholder_name())
	{
		load_presets();
	}

	bool imgui_dropdown(const char* label, T* setting_values, std::function<void()> on_value_change) {
		bool b = false;
		if (ImGui::BeginCombo(label, current_name().c_str(), 0)) {
			for (size_t i = 0; i < _presets.size(); i++) {
				if (ImGui::Selectable(_presets[i].name.c_str(), false)) {
					if (i != _current_preset_idx) {
						const auto cur_setting_values = *setting_values;
						const auto new_setting_values = _presets[i].values_with_uuid.values;
						const auto cur_uuid = current_uuid();
						const auto new_uuid = _presets[i].values_with_uuid.uuid;
						const auto last_uuid_copy = last_uuid();
						const Action action = {
							[&, setting_values, new_setting_values, new_uuid, on_value_change]() {
								*setting_values = new_setting_values;
								on_value_change();
								compute_current_preset_idx(new_uuid);
								_last_uuid = new_uuid;
							},
							[&, setting_values, cur_setting_values, cur_uuid, last_uuid_copy, on_value_change]() {
								*setting_values = cur_setting_values;
								on_value_change();
								compute_current_preset_idx(cur_uuid);
								_last_uuid = last_uuid_copy;
							}
						};
						action.apply();
						ParametersHistory::get().begin_undo_group();
						ParametersHistory::get().add_action(action);
						ParametersHistory::get().end_undo_group();
						b = true;
					}
				}
			}
			ImGui::EndCombo();
		}
		return b;
	}

	bool imgui(T* setting_values, std::function<void()> on_value_change) {
		bool b = imgui_dropdown("Presets", setting_values, on_value_change);
		// Save preset
		if (_current_preset_idx == -1) {
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
				_name_available = !File::exists(full_path(_save_preset_as)) && _save_preset_as.compare(_placeholder_name);
				_name_contains_dots = _save_preset_as.find(".") != std::string::npos;
			}
			ImGui::PopID();
			_rename_popup_open_this_frame = false;
		}
		// Rename / Delete preset
		else {
			ImGui::invisible_wrapper_around_previous_line("ovsidhsdh"); // Necessary otherwise we can't open a context menu on the dropdown
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::begin_popup_context_menu_from_button("Rename")) {
					if (!_rename_popup_open_last_frame) {
						_new_preset_name = current_name();
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
					if (boxer::show(("\"" + current_name() + "\" will be deleted. Are you sure ?").c_str(), "Delete", boxer::Style::Warning, boxer::Buttons::YesNo) == boxer::Selection::Yes) {
						std::filesystem::remove(full_path(current_name()));
						_presets.erase(_current_preset_idx + _presets.begin());
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
		_current_preset_idx = -1;
	}

private:
	std::string full_path(const std::string& file_name) {
		return _folder_path + "/" + _file_extension + file_name + ".json";
	}
	void sort() {
		const auto uuid = current_uuid();
		// Case insensitive alphabetical sort
		std::sort(_presets.begin(), _presets.end(), [](const Preset<T>& l, const Preset<T>& r) {
			return String::to_lower(l.name) < String::to_lower(r.name);
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
		compute_current_preset_idx(uuid);
	}

	void on_rename_popup_close() {
		_rename_popup_open_this_frame = false;
		if (_rename_popup_open_last_frame) {
			if (_new_preset_name.find(".") == std::string::npos) {
				const std::string new_path = full_path(_new_preset_name);
				if (!File::exists(new_path)) {
					std::filesystem::rename(
						full_path(current_name()),
						new_path
					);
					_presets[_current_preset_idx].name = _new_preset_name;
					sort();
				}
				else {
					boxer::show(("\"" + _new_preset_name + "\" already exists.").c_str(), "Renaming failed", boxer::Style::Warning);
				}
			}
			else {
				boxer::show("You can't have dots (.) in the name.", "Renaming failed", boxer::Style::Warning);
			}
		}
	}

	void compute_current_preset_idx(long int uuid) {
		_current_preset_idx = -1;
		for (size_t i = 0; i < _presets.size(); ++i) {
			if (_presets[i].values_with_uuid.uuid == uuid) {
				_current_preset_idx = i;
				break;
			}
		}
	}

	std::string find_placeholder_name() {
		int i = 1;
		std::string name = "MyPreset";
		while (File::exists(full_path(name))) {
			name = "MyPreset(" + std::to_string(i) + ")";
			i++;
		}
		return name;
	}

	void load_presets() { 
		if (File::exists(_folder_path)) {
			for (const auto& file : std::filesystem::directory_iterator(_folder_path)) {
				if (!file.path().filename().replace_extension("").replace_extension(".").string().compare(_file_extension)) {
					std::string name = file.path().filename().replace_extension("").extension().string().erase(0, 1);
					ValuesWithUUID<T> values_with_uuid;
					std::ifstream is(file.path());
					try {
						cereal::JSONInputArchive archive(is);
						archive(
							values_with_uuid
						);
						_presets.push_back({ name, values_with_uuid.values, values_with_uuid.uuid });
					}
					catch (std::exception e) {
						Log::ToUser::warn("[Load Preset] Invalid file '{}'\n{}", file.path().string(), e.what());
					}
				}
			}
			sort();
		}
	}

	void save_preset(T& settingValues) {
		// Add it to current list
		_presets.push_back({ _save_preset_as, settingValues });
		// Set as selected preset
		_current_preset_idx = _presets.size() - 1;
		// Set as last uuid
		set_last_uuid(_presets.back().values_with_uuid.uuid);
		// Save to file
		Serialization::to_json(_presets.back().values_with_uuid, full_path(_save_preset_as));
		// Sort
		sort();
		// Find new placeholder name
		_save_preset_as = find_placeholder_name();
	}

	const std::string& current_name() const {
		return _current_preset_idx != -1 ? _presets[_current_preset_idx].name : _placeholder_name;
	}

	long int current_uuid() const {
		return _current_preset_idx != -1 ? _presets[_current_preset_idx].values_with_uuid.uuid : 0;
	}

	long int last_uuid() const {
		return _last_uuid;
	}

	void discard_last_uuid() {
		_last_uuid = 0;
	}

	void set_last_uuid(long int uuid) {
		_last_uuid = uuid;
	}

private:
	const std::string      _file_extension;
	const std::string      _folder_path; // Must be declared before _save_preset_as
	size_t                 _current_preset_idx = -1;
	long int               _last_uuid = 0;
	std::vector<Preset<T>> _presets;
	std::string            _save_preset_as;
	bool                   _name_available = true;
	bool                   _name_contains_dots = false;
	std::string            _new_preset_name;
	bool                   _rename_popup_open_last_frame = false;
	bool                   _rename_popup_open_this_frame = false;
	static inline const std::string _placeholder_name = "Unsaved settings...";

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::make_nvp("Current Preset uuid", current_uuid())
		);
	}
	template <class Archive>
	void load(Archive& archive)
	{
		long int preset_uuid;
		archive(
			preset_uuid
		);
		compute_current_preset_idx(preset_uuid);
		_last_uuid = preset_uuid;
	}
};

} // namespace Cool