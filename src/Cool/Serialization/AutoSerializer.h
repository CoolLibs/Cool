#pragma once
#include "as_json.h"

namespace Cool {

template<typename T>
class AutoSerializer {
public:
    AutoSerializer(std::filesystem::path path, std::string_view key_name_in_json, T& object_to_serialize, bool is_allowed_to_throw = false, bool load_from_file = true)
        : _path{path}
        , _key_name_in_json{key_name_in_json}
        , _object_to_serialize{object_to_serialize}
    {
        if (load_from_file)
        {
            Serialization::from_json(_object_to_serialize, _path, is_allowed_to_throw);
        }
    }

    ~AutoSerializer()
    {
        Serialization::to_json(_object_to_serialize, _path, _key_name_in_json);
    }

    auto path() const { return _path; }

private:
    std::filesystem::path _path;
    std::string           _key_name_in_json;
    T&                    _object_to_serialize;
};

} // namespace Cool
