#pragma once

#include "../ParametersHistory.h"

namespace Cool {

/**
 * @brief A parameter that holds a value of type T. Groups all behaviours common to all parameters (All changes to the value are saved in an history)
 * 
 * @tparam T 
 */
template<typename Desc>
class Parameter_Base {
public:
    Parameter_Base(const Desc& desc = {})
        : _desc{desc}, _value{desc.default_value}, _value_before_edit{desc.default_value}
    {
    }

    Desc::Out operator*() const { return _desc.value(_value); }
    // const Value* operator->() const { return &_value; }

    bool imgui(
        Action on_edit_ended = {}, std::function<void()> on_value_change = []() {});

    const std::string& name() const { return _desc.name; }

protected:
    void push_change_in_history_if_edit_ended(Action on_edit_ended, std::function<void()> on_value_change);

private:
    void push_change_in_history(Action on_edit_ended, std::function<void()> on_value_change);

private:
    Desc      _desc;
    Desc::Rep _value;
    Desc::Rep _value_before_edit;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(cereal::make_nvp("Description", _desc),
                cereal::make_nvp("Value", _value));
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(_desc,
                _value);
        _value_before_edit = _value;
    }
};

} // namespace Cool

#include "Parameter_Base.tpp"