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
class ParameterGroup {
public:
	ParameterGroup(const std::string& file_extension, const std::string& folder_path, std::function<void()> on_value_change = []() {})
		: _presets(file_extension, folder_path), _on_value_change(on_value_change)
	{}
	bool ImGui() {
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
		if (_values.ImGui(action, _on_value_change)) {
			_presets.set_to_placeholder_setting();
			b = true;
		}
		ImGui::Separator();
		if (b |= _presets.ImGui(&_values, _on_value_change))
			_on_value_change();
		return b;
	}
	inline const ParamValues& operator* () const { return  _values; }
	inline const ParamValues* const operator->() const { return &_values; }

private:
	ParamValues _values;
	Presets<ParamValues> _presets;
	std::function<void()> _on_value_change;

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