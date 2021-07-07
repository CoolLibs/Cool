#pragma once

#include "../ParametersHistory.h"
#include "IParameter.h"

namespace Cool::Internal {

/**
 * @brief A parameter that holds a value of type T. Groups all behaviours common to all parameters (All changes to the value are saved in an history)
 * 
 * @tparam T 
 */
template <typename T>
class Parameter : public IParameter {
public:
	Parameter(std::string_view name, const T& default_value = T(0))
		: _name(name), _value(default_value), _value_before_edit(default_value)
	{}
	virtual ~Parameter() = default;
	inline const T& operator* () const { return _value; }
	inline const T* const operator->() const { return &_value; }

	bool imgui(Action on_edit_ended = {}, std::function<void()> on_value_change = [](){}) override {
		bool b = imgui_widget();
		push_change_in_history_if_edit_ended(on_edit_ended, on_value_change);
		if (b) {
			on_value_change();
		}
		return b;
	}

	inline const std::string& name() const override { return _name; }

	inline void set_uniform_in_shader(Shader& shader) override { shader.set_uniform(name(), _value); }

protected:
	virtual bool imgui_widget() = 0;
	void push_change_in_history_if_edit_ended(Action on_edit_ended, std::function<void()> on_value_change) {
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			push_change_in_history(on_edit_ended, on_value_change);
			on_edit_ended.apply();
			_value_before_edit = _value; // ready for next edit
		}
	}

private:
	void push_change_in_history(Action on_edit_ended, std::function<void()> on_value_change) {
		ParametersHistory::get().begin_undo_group();
		T val = _value;
		T prev_val = _value_before_edit;
		ParametersHistory::get().add_action({
			[&, val, on_value_change]()
			{
				Log::info("++"); // TODO remove me
				_value = val;
				on_value_change();
			},
			[&, prev_val, on_value_change]()
			{
				Log::info("--"); // TODO remove me
				_value = prev_val;
				on_value_change();
			}
		});
		ParametersHistory::get().add_action(on_edit_ended);
		ParametersHistory::get().end_undo_group();
	}

protected:
	T _value;
	T _value_before_edit;

private:
	std::string _name;

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::make_nvp("Name", _name),
			cereal::make_nvp("Value", _value)
		);
	}
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			_name,
			_value
		);
		_value_before_edit = _value;
	}
};

} // namespace Cool::Internal