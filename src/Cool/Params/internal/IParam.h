#pragma once

#include "../ParamsHistory.h"

namespace Cool::Internal {

class IParam {
public:
	IParam(std::string_view name)
		: _name(name)
	{}
	virtual ~IParam() = default;

	virtual bool ImGui() = 0;

	[[nodiscard]] inline const std::string& name() const { return _name; }

private:
	std::string _name = "";
};

template <typename T>
class Param : public IParam {
public:
	Param(std::string_view name, const T& default_value = T(0))
		: IParam(name), _value(default_value), _value_before_edit(default_value)
	{}
	inline T& operator* () { return  _value; }
	inline T* operator->() { return &_value; }

protected:
	T _value;
	T _value_before_edit;

protected:
	void push_change_in_history_if_edit_ended() {
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			push_change_in_history();
			_value_before_edit = _value; // ready for next edit
		}
	}

private:
	void push_change_in_history() {
		ParamsHistory::Get().begin_undo_group();
		T val = _value;
		T prev_val = _value_before_edit;
		ParamsHistory::Get().add_action({
			[&, val]()
			{
				_value = val;
			},
			[&, prev_val]()
			{
				_value = prev_val;
			}
		});
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