#pragma once

#include "internal/Presets.h"
// Include all Para types so that people only need to include this one file
#include "internal/Param_Color.h"

namespace Cool {

template <typename ParamValues>
class Params {
public:
	Params(const std::string& file_extension, const std::string& folder_path)
		: _presets(file_extension, folder_path)
	{}
	bool ImGui() {
		bool b = false;
		if (_values.ImGui()) {
			_presets.setToPlaceholderSetting();
			b = true;
		}
		b |= _presets.ImGui(&_values);
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