#pragma once
#include <Cool/FileWatcher/FileWatcher.h>
#include <Cool/Log/MessageSender.h>
#include <filesystem>
#include <reg/reg.hpp>
#include "Dirty.h"
#include "VariableId.h"
#include "VariableRegistries.h"

namespace Cool {

template<typename T>
class Input {
public:
    Input() = default; // For serialization

    auto name() const -> const std::string& { return _name; }

    void set_dirty_if_depends_on(const VariableId<T>& variable_id, SetDirty_Ref set_dirty, bool use_secondary_dirty_flag = false) const
    {
        if (_current_variable_id == variable_id
            || (_current_variable_id.underlying_uuid().is_nil() && _default_variable_id.raw() == variable_id))
        {
            set_dirty(use_secondary_dirty_flag ? _secondary_dirty_flag : _dirty_flag);
        }
    }

private:
    friend class InputFactory_Ref;
    explicit Input( // Use InputFactory_Ref::make() to create an input
        DirtyFlag                         dirty_flag,
        DirtyFlag                         secondary_dirty_flag,
        std::string_view                  name,
        const std::optional<std::string>& description,
        SharedVariableId<T>               default_variable_id,
        int                               desired_color_space
    )
        : _dirty_flag{std::move(dirty_flag)}
        , _secondary_dirty_flag{std::move(secondary_dirty_flag)}
        , _name{name}
        , _description{description}
        , _default_variable_id{default_variable_id}
        , _desired_color_space{desired_color_space}
    {
    }

public: // private: TODO(JF) make this private
    friend class InputProvider_Ref;
    friend class Ui_Ref;
    DirtyFlag                  _dirty_flag;
    DirtyFlag                  _secondary_dirty_flag;
    std::string                _name; // NB: both the Input and the Variables have a name, because they are two different things and the current variable could be coming from a global pool and not be related to this Input at all.
    std::optional<std::string> _description;
    SharedVariableId<T>        _default_variable_id; // TODO(JF) Can't it be unique instead of shared?
    VariableId<T>              _current_variable_id;
    int                        _desired_color_space{0}; // HACK in order to know which color space to convert to when sending the value to a shader. Only used by Color input.

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", _name),
            // cereal::make_nvp("Description", _description), // (JF): I don't think there is a need to serialize the description since it will be parsed from the shader each time, and applying presets and the like only affect the value of the variable.
            cereal::make_nvp("Default Variable ID", _default_variable_id),
            cereal::make_nvp("Current Variable ID", _current_variable_id),
            cereal::make_nvp("Dirty Flag", _dirty_flag),
            cereal::make_nvp("desired color space", _desired_color_space)
        );
    }
};

struct Input_AspectRatio {
};
struct Input_Height {
};
struct Input_Camera2D {
};
struct Input_Time {
};
struct Input_DeltaTime {
};
struct Input_Audio {
};

#include <Cool/Variables/generated/AnyInput.inl>
#include <Cool/Variables/generated/AnyInputRef.inl>
#include <Cool/Variables/generated/AnyInputRefToConst.inl>

using AllInputRefsToConst = std::vector<AnyInputRefToConst>;

} // namespace Cool