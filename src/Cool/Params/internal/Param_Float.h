#pragma once

#include "IParam.h"

namespace Cool::Param {

class Float : public Internal::Param<float> {
public:
	Float(std::string_view name, float default_value = 0.f, float min_value = 0.f, float max_value = 1.f)
		: Param(name, default_value), _min_value(min_value), _max_value(max_value)
	{}

protected:
	bool ImGuiWidget() override {
		bool b = ImGui::SliderFloat(name().c_str(), &_value, _min_value, _max_value);
		ImGui::PushID(this);
		if (ImGui::BeginPopupContextItem()) {
			ImGui::DragFloat("", &_min_value);
			ImGui::SameLine();
			ImGui::Text("to");
			ImGui::SameLine();
			ImGui::DragFloat(" ", &_max_value);
			ImGui::EndPopup();
		}
		ImGui::PopID();
		return b;
	}

private:
	float _min_value;
	float _max_value;

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Internal::Param<float>>(this),
			cereal::make_nvp("Min Value", _min_value),
			cereal::make_nvp("Max Value", _max_value)
		);
	}
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Internal::Param<float>>(this),
			_min_value,
			_max_value
		);
	}
};

} // namespace Cool::Param