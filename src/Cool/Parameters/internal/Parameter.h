#pragma once

#include <Cool/Gpu/OpenGL/Shader.h>
#include "../ParametersHistory.h"

namespace Cool::Parameter {

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
        Action on_edit_ended = {}, std::function<void()> on_value_change = []() {})
    {
        bool b = _desc.imgui(_value);
        push_change_in_history_if_edit_ended(on_edit_ended, on_value_change);
        if (b) {
            on_value_change();
        }
        return b;
    }

    const std::string& name() const { return _desc.name; }

#if defined(COOL_OPENGL)
    void set_uniform_in_shader(const OpenGL::Shader& shader) const
    {
        shader.set_uniform(name(), operator*());
    }
#endif

protected:
    void push_change_in_history_if_edit_ended(Action on_edit_ended, std::function<void()> on_value_change)
    {
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            push_change_in_history(on_edit_ended, on_value_change);
            on_edit_ended.apply();
            _value_before_edit = _value; // ready for next edit
        }
    }

private:
    void push_change_in_history(Action on_edit_ended, std::function<void()> on_value_change)
    {
        ParametersHistory::get().begin_undo_group();
        const auto val      = _value;
        const auto prev_val = _value_before_edit;
        ParametersHistory::get().add_action({[&, val, on_value_change]() {
                                                 _value = val;
                                                 on_value_change();
                                             },
                                             [&, prev_val, on_value_change]() {
                                                 _value = prev_val;
                                                 on_value_change();
                                             }});
        ParametersHistory::get().add_action(on_edit_ended);
        ParametersHistory::get().end_undo_group();
    }

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

} // namespace Cool::Parameter