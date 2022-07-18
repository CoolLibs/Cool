#pragma once
#include "as_json.h"

namespace Cool {

template<typename T>
class AutoSerializer {
public:
    AutoSerializer(std::string_view path, std::string_view key_name_in_json, T& object_to_serialize, bool load_from_file = true)
        : _path{path}
        , _key_name_in_json{key_name_in_json}
        , _object_to_serialize{object_to_serialize}
    {
        if (load_from_file)
        {
            Serialization::from_json(_object_to_serialize, _path, true);
        }
    }

    ~AutoSerializer()
    {
        Serialization::to_json(_object_to_serialize, _path, _key_name_in_json);
    }

private:
    std::string _path;
    std::string _key_name_in_json;
    T&          _object_to_serialize;
};

} // namespace Cool
