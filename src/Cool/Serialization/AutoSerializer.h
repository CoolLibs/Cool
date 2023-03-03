#pragma once
#include <concepts>
#include "Cool/Log/OptionalErrorMessage.h"
#include "as_json.h"

namespace Cool {

template<typename T>
class AutoSerializer {
public:
    template<std::invocable<OptionalErrorMessage const&> OnError>
    AutoSerializer(
        std::filesystem::path const& path,
        std::string_view             key_name_in_json,
        T&                           object_to_serialize,
        OnError&&                    on_error,
        bool                         load_from_file = true
    )
        : _d{
            .path                = path,
            .key_name_in_json    = std::string{key_name_in_json},
            .object_to_serialize = object_to_serialize,
        }
    {
        if (!load_from_file)
            return;

        const auto error = Serialization::from_json(_d.object_to_serialize.get(), _d.path);
        if (error)
            on_error(error);
    }

    ~AutoSerializer()
    {
        if (!_has_been_moved_from)
            save();
    }

    AutoSerializer(AutoSerializer<T> const&)                    = delete;
    auto operator=(AutoSerializer<T> const&) -> AutoSerializer& = delete;
    AutoSerializer(AutoSerializer<T>&& o) noexcept
        : _d{std::move(o._d)}
    {
        o._has_been_moved_from = true;
    }
    auto operator=(AutoSerializer<T>&& o) noexcept -> AutoSerializer&
    {
        _d                     = std::move(o._d);
        o._has_been_moved_from = true;
        return *this;
    }

    void save() const
    {
        Serialization::to_json(_d.object_to_serialize.get(), _d.path, _d.key_name_in_json);
    }

    auto path() const { return _d.path; }

private:
    struct Data { // Needed to simplify writing our move operators
        std::filesystem::path     path;
        std::string               key_name_in_json;
        std::reference_wrapper<T> object_to_serialize;
    };
    Data _d;
    bool _has_been_moved_from{false};
};

} // namespace Cool
