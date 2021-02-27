#pragma once

namespace Cool {

template <typename T>
class _Preset {
public:
	_Preset(T values, std::string_view name)
		: _values(values), _name(name)
	{}

	inline const std::string& name() const { return _name; }
	inline const T& values() const { return _values; }

private:
	T _values;
	std::string _name;
};

template <typename T>
class Presets {
public:
	Presets() {
		_preset_values.emplace_back(T(), "Default");
		_preset_values.emplace_back(T(), "Default2");
	}

	inline const T& operator*() const { return _values; }
	inline const T* operator->() const { return &_values; }

	void ImGui() {
		ImGuiDropdown("Hello");
	}

	bool ImGuiDropdown(const char* label) {
		bool b = false;
		if (ImGui::BeginCombo(label, _current_preset_idx == - 1 ? "Unsaved preset" : _preset_values[_current_preset_idx].name().c_str(), 0)) {
			for (size_t i = 0; i < _preset_values.size(); i++) {
				if (ImGui::Selectable(_preset_values[i].name().c_str(), false)) {
					b = true;
					_current_preset_idx = i;
					_values = _preset_values[i].values();
				}
			}
			ImGui::EndCombo();
		}
		return b;
	}

private:
	T _values;
	std::vector<_Preset<T>> _preset_values;
	size_t _current_preset_idx = -1;
};

} // namespace Cool