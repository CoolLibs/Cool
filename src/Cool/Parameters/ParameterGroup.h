#pragma once

#include "internal/Presets.h"
#include "ParametersHistory.h"
#include "AnyParameter.h"

namespace Cool {

/**
 * @brief Wraps a struct made of Parameter::xxx and handles presets for that struct
 * 
 * @tparam ParameterValues All fields of this struct must be a Parameter of any type (Parameter::Float, Parameter::Color etc.)
 */
template <typename ParameterValues>
class ParameterGroup {
public:
	ParameterGroup(const std::string& file_extension, const std::string& folder_path, std::function<void()> on_value_change = []() {})
		: _presets(file_extension, folder_path)
		, _on_value_change(on_value_change)
	{}

	bool imgui() {
		const auto uuid = _presets.last_uuid();
		const Action action = {
			[&]() {
				_presets.set_to_placeholder_setting();
				_presets.discard_last_uuid();
			},
			[&, uuid]() {
				_presets.compute_current_preset_idx(uuid);
				_presets.set_last_uuid(uuid);
			},
		};
		bool b = false;
		if (_values.imgui(action, _on_value_change)) {
			_presets.set_to_placeholder_setting();
			b = true;
		}
		ImGui::Separator();
		if (b |= _presets.imgui(&_values, _on_value_change)) {
			_on_value_change();
		}
		return b;
	}
	inline const ParameterValues& operator* () const { return  _values; }
	inline const ParameterValues* const operator->() const { return &_values; }

private:
	ParameterValues          _values;
	Presets<ParameterValues> _presets;
	std::function<void()>    _on_value_change;

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			_values,
			_presets
		);
	}
};

} // namespace Cool