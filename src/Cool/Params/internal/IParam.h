#pragma once

#include "../ParamsHistory.h"

#ifdef __COOL_OPENGL
namespace Cool {
	class Shader;
}
#endif

namespace Cool::Internal {

class IParam {
public:
	IParam(std::string_view name)
		: _name(name)
	{}
	virtual ~IParam() = default;

	virtual bool ImGui(Action on_edit_ended, std::function<void()> on_value_change = []() {}) = 0;

#ifdef __COOL_OPENGL
	virtual void set_uniform_in_shader(Shader& shader) = 0;
#endif

	[[nodiscard]] inline const std::string& name() const { return _name; }

protected:
	virtual bool ImGuiWidget() = 0;

private:
	std::string _name = "";
};

template <typename T>
class Param : public IParam {
public:
	Param(std::string_view name, const T& default_value = T(0))
		: IParam(name), _value(default_value), _value_before_edit(default_value)
	{}
	inline const T& operator* () const { return  _value; }
	inline const T* const operator->() const { return &_value; }

	bool ImGui(Action on_edit_ended, std::function<void()> on_value_change) override {
		bool b = ImGuiWidget();
		push_change_in_history_if_edit_ended(on_edit_ended, on_value_change);
		if (b)
			on_value_change();
		return b;
	}

#ifdef __COOL_OPENGL
	void set_uniform_in_shader(Shader& shader) override {
		shader.setUniform(name().c_str(), _value);
	}
#endif

protected:
	T _value;
	T _value_before_edit;

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
		ParamsHistory::Get().begin_undo_group();
		T val = _value;
		T prev_val = _value_before_edit;
		ParamsHistory::Get().add_action({
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
		ParamsHistory::Get().add_action(on_edit_ended);
		ParamsHistory::Get().end_undo_group();
	}

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::make_nvp(name(), _value)
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