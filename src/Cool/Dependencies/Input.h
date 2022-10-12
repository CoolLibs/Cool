#pragma once
#include <Cool/FileWatcher/FileWatcher.h>
#include <Cool/Log/MessageSender.h>
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

    void set_dirty_if_depends_on(const VariableId<T>& variable_id, SetDirty_Ref set_dirty) const
    {
        if (_current_variable_id == variable_id ||
            (_current_variable_id.underlying_uuid().is_nil() && _default_variable_id == variable_id))
        {
            set_dirty(_dirty_flag);
        }
    }

private:
    friend class InputFactory_Ref;
    explicit Input( // Use InputFactory_Ref::make() to create an input
        const DirtyFlag&                  dirty_flag,
        std::string_view                  name,
        const std::optional<std::string>& description,
        const VariableId<T>               default_variable_id
    )
        : _dirty_flag{dirty_flag}
        , _name{name}
        , _description{description}
        , _default_variable_id{default_variable_id}
    {
    }

public: // private: TODO(JF) make this private
    friend class InputProvider_Ref;
    friend class Ui_Ref;
    friend class InputDestructor_Ref;
    DirtyFlag                  _dirty_flag;
    std::string                _name; // NB: both the Input and the Variables have a name, because they are two different things and the current variable could be coming from a global pool and not be related to this Input at all.
    std::optional<std::string> _description;
    VariableId<T>              _default_variable_id;
    VariableId<T>              _current_variable_id;

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
            cereal::make_nvp("Dirty Flag", _dirty_flag)
        );
    }
};

class Input_File {
public:
    Input_File() = default; // For serialization :( Remove whenever possible
    explicit Input_File(DirtyFlag dirty_flag)
        : _dirty_flag{dirty_flag}
    {
    }

    void update(SetDirty_Ref set_dirty)
    {
        file_watcher.update(
            {
                .on_file_changed =
                    [&](std::string_view) {
                        set_dirty(_dirty_flag);
                        _path_error.clear();
                    },
                .on_path_invalid =
                    [&](std::string_view path) {
                        _path_error.send(
                            {
                                .category = "Input File",
                                .message  = fmt::format("Invalid path: \"{}\"", path),
                                .severity = Cool::MessageSeverity::Error,
                            }
                        );
                    },
            }
        );
    }

    auto should_highlight() const -> bool
    {
        return _path_error.should_highlight();
    }

public: // private: // TODO(JF) Make this private!
    friend class Ui_Ref;
    friend class InputProvider_Ref;
    friend class InputDestructor_Ref;
    Cool::FileWatcher file_watcher{};
    DirtyFlag         _dirty_flag;

private:
    Cool::MessageSender _path_error{};

private:
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("File Path", file_watcher.path()),
            _dirty_flag
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        std::filesystem::path path;
        archive(
            path,
            _dirty_flag
        );
        file_watcher.set_path(path);
    }
};

struct Input_AspectRatio {
};
struct Input_Time {
};

#include <Cool/Variables/generated/AnyInput.inl>
#include <Cool/Variables/generated/AnyInputRef.inl>
#include <Cool/Variables/generated/AnyInputRefToConst.inl>

class InputDestructor_Ref {
public:
    explicit InputDestructor_Ref(VariableRegistries& registries)
        : _variable_registries{registries}
    {
    }

    void operator()(const AnyInput& input)
    {
        std::visit([&](auto&& input) { _variable_registries.get().destroy(input._default_variable_id); }, input);
    }

private:
    std::reference_wrapper<VariableRegistries> _variable_registries;
};

using AllInputRefsToConst = std::vector<AnyInputRefToConst>;

} // namespace Cool