#pragma once
#include <concepts>
#include "Cool/Log/OptionalErrorMessage.h"
#include "as_json.h"

namespace Cool {

class AutoSerializer {
public:
    AutoSerializer() = default;
    template<typename T, typename InputArchive>
    void init(
        std::filesystem::path const&                            path,
        T&                                                      object_to_serialize,
        std::function<void(OptionalErrorMessage const&)> const& on_error,
        std::function<void(std::filesystem::path const&)>       on_save,
        bool                                                    load_from_file = true
    )
    {
        _d = {
            .path    = path,
            .on_save = std::move(on_save),
        };
        if (!load_from_file)
            return;

        auto const error = Serialization::from_json<T, InputArchive>(object_to_serialize, _d.path);
        if (error)
            on_error(error);
    }

    ~AutoSerializer()
    {
        if (!_has_been_moved_from)
            save();
    }

    AutoSerializer(AutoSerializer const&)                    = delete;
    auto operator=(AutoSerializer const&) -> AutoSerializer& = delete;
    AutoSerializer(AutoSerializer&& o) noexcept
        : _d{std::move(o._d)}
    {
        o._has_been_moved_from = true;
    }
    auto operator=(AutoSerializer&& o) noexcept -> AutoSerializer&
    {
        if (&o == this)
            return *this;

        _d                     = std::move(o._d);
        o._has_been_moved_from = true;
        return *this;
    }

    void save() const
    {
        _d.on_save(_d.path);
    }

    [[nodiscard]] auto path() const { return _d.path; }

private:
    struct Data { // Needed to simplify writing our move operators
        std::filesystem::path                             path;
        std::function<void(std::filesystem::path const&)> on_save;
    };
    Data _d;
    bool _has_been_moved_from{false};
};

} // namespace Cool
