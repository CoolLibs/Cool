#pragma once

#include <Cool/File/File.h>
#include <Cool/String/String.h>
#include <fstream>
#include <Cool/Serialization/JsonFile.h>
#include <Cool/ImGui/ImGui.h>

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
		  m_savePresetAs(findPlaceholderName())
	{
		loadPresetsFrom(_folder_path);
	}
	~Presets() = default;

	inline const std::string& getPresetName() { return m_currentPresetName; }
	inline void setPresetName(const std::string& newName) { m_currentPresetName = newName; }

	bool ImGuiDropdown(const char* label, T* settingValues) {
		bool b = false;
		if (ImGui::BeginCombo(label, m_currentPresetName.c_str(), 0)) {
			for (size_t i = 0; i < m_presets.size(); i++) {
				if (ImGui::Selectable(m_presets[i].name.c_str(), false)) {
					b = true;
					m_currentPresetName = m_presets[i].name;
					m_currentPresetIdx = i;
					*settingValues = m_presets[i].values;
				}
			}
			ImGui::EndCombo();
		}
		return b;
	}

	bool ImGui(T* settingValues) {
		ImGui::Separator();
		bool b = ImGuiDropdown("Presets", settingValues);
		// Save preset
		if (!m_currentPresetName.compare("Unsaved settings...")) {
			if (m_nameAvailable) {
				if (!m_nameContainsDots) {
					if (ImGui::Button("Save settings")) {
						savePresetTo(*settingValues, _folder_path);
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
			if (ImGui::InputText("", &m_savePresetAs)) {
				m_nameAvailable = !File::Exists(_folder_path + "/" + _file_extension + m_savePresetAs + ".json") && m_savePresetAs.compare("Unsaved settings...");
				m_nameContainsDots = m_savePresetAs.find(".") != std::string::npos;
			}
			ImGui::PopID();
			m_bRenamePopupOpenThisFrame = false;
		}
		// Rename / Delete preset
		else {
			if (CoolImGui::BeginPopupContextMenuFromButton("Rename")) {
				if (!m_bRenamePopupOpenLastFrame) {
					m_newPresetName = m_currentPresetName;
				}
				ImGui::PushID(16879654123594);
				ImGui::InputText("", &m_newPresetName);
				ImGui::PopID();
				ImGui::EndPopup();
				m_bRenamePopupOpenThisFrame = true;
			}
			else {
				m_bRenamePopupOpenThisFrame = false;
				if (m_bRenamePopupOpenLastFrame) {
					computeCurrentPresetIdx(); // Could have been changed by playing a record
					if (m_currentPresetIdx != -1) {
						const std::string newPath = _folder_path + "/" + _file_extension + m_newPresetName + ".json";
						if (!File::Exists(newPath)) {
							std::filesystem::rename(
								_folder_path + "/" + _file_extension + m_currentPresetName + ".json",
								newPath
							);
							m_currentPresetName = m_newPresetName;
							m_presets[m_currentPresetIdx].name = m_newPresetName;
							sort();
						}
						else {
							boxer::show(("\"" + m_newPresetName + "\" already exists.").c_str(), "Renaming failed", boxer::Style::Warning);
						}
					}
					else {
						boxer::show("Actually this preset isn't saved on your computer, it was just set by a record.\nYou will now be able to \"Save settings\" if you want to.", "Renaming failed", boxer::Style::Info);
						findPlaceholderName();
						setToPlaceholderSetting();
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete")) {
				if (boxer::show(("\"" + m_currentPresetName + "\" will be deleted. Are you sure ?").c_str(), "Delete", boxer::Style::Warning, boxer::Buttons::YesNo) == boxer::Selection::Yes) {
					computeCurrentPresetIdx(); // Could have been changed by playing a record
					if (m_currentPresetIdx != -1) {
						std::filesystem::remove(
							_folder_path + "/" + _file_extension + m_currentPresetName + ".json"
						);
						m_presets.erase(m_currentPresetIdx + m_presets.begin());
					}
					else {
						boxer::show("Actually this preset isn't saved on your computer, it was just set by a record.\nYou will now be able to \"Save settings\" if you want to.", "Deleting failed", boxer::Style::Info);
					}
					findPlaceholderName();
					setToPlaceholderSetting();
				}
			}
		}
		m_bRenamePopupOpenLastFrame = m_bRenamePopupOpenThisFrame;
		return b;
	}

	void setToPlaceholderSetting() {
		m_currentPresetName = "Unsaved settings...";
		m_currentPresetIdx = -1;
	}

private:
	void sort() {
		// Case insensitive alphabetical sort
		std::sort(m_presets.begin(), m_presets.end(), [](const Preset<T>& l, const Preset<T>& r) {
			return String::ToLower(l.name) < String::ToLower(r.name);
			});
		// Put Default first
		for (size_t i = 0; i < m_presets.size(); ++i) {
			if (!m_presets[i].name.compare("Default")) {
				Preset<T> prst = m_presets[i];
				std::copy_backward(m_presets.begin(), i + m_presets.begin(), i + 1 + m_presets.begin());
				m_presets[0] = prst;
				break;
			}
		}
		//
		computeCurrentPresetIdx();
	}

	void computeCurrentPresetIdx() {
		for (size_t i = 0; i < m_presets.size(); ++i) {
			if (!m_presets[i].name.compare(m_currentPresetName)) {
				m_currentPresetIdx = i;
				break;
			}
		}
	}

	std::string findPlaceholderName() {
		int i = 1;
		std::string name = "MyPreset";
		while (File::Exists(_folder_path + "/" + _file_extension + name + ".json")) {
			name = "MyPreset(" + std::to_string(i) + ")";
			i++;
		}
		return name;
	}

	void loadPresetsFrom(const std::string& folderPath) {
		if (File::Exists(folderPath)) {
			for (const auto& file : std::filesystem::directory_iterator(folderPath)) {
				if (!file.path().filename().replace_extension("").replace_extension(".").string().compare(_file_extension)) {
					std::string name = file.path().filename().replace_extension("").extension().string().erase(0, 1);
					T values;
					std::ifstream is(file.path());
					{
						cereal::JSONInputArchive archive(is);
						archive(
							values
						);
					}
					m_presets.push_back({ name, values });
				}
			}
			sort();
		}
	}

	void savePresetTo(T& settingValues, const std::string& folderPath) {
		File::CreateFoldersIfDoesntExist(folderPath);
		std::ofstream os(folderPath + "/" + _file_extension + m_savePresetAs + ".json");
		{
			cereal::JSONOutputArchive archive(os);
			archive(
				settingValues
			);
		}
		// Add it to current list
		m_presets.push_back({ m_savePresetAs, settingValues });
		// Give the name to the selected preset
		m_currentPresetName = m_savePresetAs;
		//
		sort();
		// Find new placeholder name
		m_savePresetAs = findPlaceholderName();
	}

private:
	const std::string _file_extension;
	const std::string _folder_path; // Must be declared before m_savePresetAs
	std::string m_currentPresetName = "Unsaved settings...";
	size_t m_currentPresetIdx = -1;
	std::vector<Preset<T>> m_presets;
	std::string m_savePresetAs;
	bool m_nameAvailable = true;
	bool m_nameContainsDots = false;
	std::string m_newPresetName;
	bool m_bRenamePopupOpenLastFrame = false;
	bool m_bRenamePopupOpenThisFrame = false;

private:
	// Serialization
	friend class cereal::access;
	template <class Archive>
	void save(Archive& archive, std::uint32_t const version) const
	{
		archive(
			CEREAL_NVP(m_currentPresetName)
		);
	}
	template <class Archive>
	void load(Archive& archive, std::uint32_t const version)
	{
		archive(
			CEREAL_NVP(m_currentPresetName)
		);
		computeCurrentPresetIdx();
	}
};
