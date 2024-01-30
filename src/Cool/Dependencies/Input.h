#pragma once
#include <Cool/Dependencies/DirtyFlag.h>
#include "Cool/Dependencies/InputDefinition.h"
#include "Cool/Variables/Variable.h"
#include "Cool/Variables/Variables.h"

namespace Cool {

template<typename T>
class Input { // TODO(Variables) Rename
public:
    Input() = default; // For serialization
    Input(
        Variable<T>                variable,
        DirtyFlag                  dirty_flag,
        std::optional<std::string> description          = {},
        DirtyFlag                  secondary_dirty_flag = {}
    )
        : _dirty_flag{std::move(dirty_flag)}
        , _secondary_dirty_flag{std::move(secondary_dirty_flag)}
        , _description{std::move(description)}
        , _variable{std::make_shared<Variable<T>>(variable)}
    {}
    Input(
        InputDefinition<T> def,
        DirtyFlag          dirty_flag,
        DirtyFlag          secondary_dirty_flag = {}
    )
        : _dirty_flag{dirty_flag}
        , _secondary_dirty_flag{secondary_dirty_flag}
        , _description{def.description}
        , _variable{std::make_shared<Variable<T>>(VariableData<T>{
              def.name,
              def.default_value,
              def.metadata,
          })}
        , _desired_color_space{def.desired_color_space}
    {}

    Input(Input const&)            = default;
    Input& operator=(Input const&) = default;

    auto name() const -> std::string const& { return _variable->name(); }
    auto value() const -> T const& { return _variable->value(); }
    auto value() -> T& { return _variable->value(); }

    auto id() const -> uintptr_t { return reinterpret_cast<uintptr_t>(_variable.get()); }
    auto variable_data() const -> VariableData<T> { return {name(), value(), _variable->metadata()}; }

    // void set_dirty_if_depends_on(const VariableId<T>& variable_id, SetDirty_Ref set_dirty, bool use_secondary_dirty_flag = false) const
    // {
    //     if (_current_variable_id == variable_id
    //         || (_current_variable_id.underlying_uuid().is_nil() && _default_variable_id.raw() == variable_id))
    //     {
    //         set_dirty(use_secondary_dirty_flag ? _secondary_dirty_flag : _dirty_flag);
    //     }
    // }

public: // private: TODO(Variable) make this private
    friend class Ui_Ref;
    DirtyFlag                    _dirty_flag;
    DirtyFlag                    _secondary_dirty_flag; // TODO(Variable) Don't we need to serialize this ?
    std::optional<std::string>   _description;
    std::shared_ptr<Variable<T>> _variable; // Shared_ptr to make sure the address stays stable in memory and we can reference it in Commands stored in the History + survive through serialization
    mutable MessageId            _message_id{};
    int                          _desired_color_space{0}; // HACK in order to know which color space to convert to when sending the value to a shader. Only used by Color input.

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            // cereal::make_nvp("Description", _description), // (JF): I don't think there is a need to serialize the description since it will be parsed from the shader each time, and applying presets and the like only affect the value of the variable.
            cereal::make_nvp("Variable", _variable),
            cereal::make_nvp("Dirty Flag", _dirty_flag),
            cereal::make_nvp("desired color space", _desired_color_space)
        );
    }
};

} // namespace Cool