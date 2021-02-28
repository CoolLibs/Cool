#pragma once

#include "internal/Presets.h"
#include "ParamsHistory.h"
// Include all Param types so that people only need to include this one file
#include "internal/Param_Color.h"

namespace Cool {

template <typename ParamValues>
class Params {
public:
	template <typename... Args>
	Params(const std::string& file_extension, const std::string& folder_path, Args&& ...args)
		: _values(std::forward<Args>(args)...), _presets(file_extension, folder_path)
	{}
	bool ImGui() {
		const auto uuid = _presets.last_uuid();
		const Action action = {
			[&]() {
				_presets.set_to_placeholder_setting();
			},
			[&, uuid]() {
				_presets.compute_current_preset_idx(uuid);
			},
		};
		bool b = false;
		if (_values.ImGui(action)) {
			_presets.set_to_placeholder_setting();
			b = true;
		}
		ImGui::Separator();
		if (b |= _presets.ImGui(&_values))
			_values.on_all_values_change();
		return b;
	}
	inline ParamValues& operator* () { return  _values; }
	inline ParamValues* operator->() { return &_values; }

private:
	ParamValues _values;
	Presets<ParamValues> _presets;

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