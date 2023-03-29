#pragma once
#include <concepts>
#include <utility>
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Serialization/as_json.h"

namespace Cool {

class SerializerOnDemand {
public:
    SerializerOnDemand(
        std::filesystem::path path,
        std::string_view      key_name_in_json
    )
        : _path{std::move(path)}
        , _key_name_in_json{key_name_in_json}
    {}

    template<typename T, typename InputArchive>
    auto load(T& object) const -> OptionalErrorMessage
    {
        return Serialization::from_json<T, InputArchive>(object, _path);
    }

    template<typename T, typename OutputArchive>
    void save(T const& object) const
    {
        Serialization::to_json<T, OutputArchive>(object, _path, _key_name_in_json);
    }

    [[nodiscard]] auto path() const -> auto const& { return _path; }

private:
    std::filesystem::path _path;
    std::string           _key_name_in_json;
};

} // namespace Cool
