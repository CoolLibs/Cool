#pragma once

#include "../ParamsHistory.h"
#include <Cool/OpenGL/Shader.h>

namespace Cool::Internal {

class IParam {
public:
	virtual ~IParam() = default;

	virtual inline const std::string& name() const = 0;
	virtual bool ImGui(Action on_edit_ended, std::function<void()> on_value_change = []() {}) = 0;
	virtual void set_uniform_in_shader(Shader& shader) = 0;

protected:
	virtual bool ImGuiWidget() = 0;
};

template <typename T>
class ParamForPrimitiveType : public IParam {
public:
	ParamForPrimitiveType(std::string_view name, const T& default_value = T(0))
		: _name(name), _value(default_value), _value_before_edit(default_value)
	{}
	inline const T& operator* () const { return _value; }
	inline const T* const operator->() const { return &_value; }

	bool ImGui(Action on_edit_ended, std::function<void()> on_value_change) override {
		bool b = ImGuiWidget();
		push_change_in_history_if_edit_ended(on_edit_ended, on_value_change);
		if (b)
			on_value_change();
		return b;
	}

	inline const std::string& name() const override { return _name; }

	void set_uniform_in_shader(Shader& shader) override {
		shader.set_uniform(name().c_str(), _value);
	}

protected:
	void push_change_in_history_if_edit_ended(Action on_edit_ended, std::function<void()> on_value_change) {
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			push_change_in_history(on_edit_ended, on_value_change);
			on_edit_ended.Do();
			_value_before_edit = _value; // ready for next edit
		}
	}

private:
	void push_change_in_history(Action on_edit_ended, std::function<void()> on_value_change) {
		ParamsHistory::get().begin_undo_group();
		T val = _value;
		T prev_val = _value_before_edit;
		ParamsHistory::get().add_action({
			[&, val, on_value_change]()
			{
				_value = val;
				on_value_change();
			},
			[&, prev_val, on_value_change]()
			{
				_value = prev_val;
				on_value_change();
			}
		});
		ParamsHistory::get().add_action(on_edit_ended);
		ParamsHistory::get().end_undo_group();
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
			cereal::make_nvp(_name, _value)
		);
	}
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			_value
		);
		_value_before_edit = _value;
	}
};

} // namespace Cool::Internal