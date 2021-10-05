#pragma once
#include "JsonFile.h"

namespace Cool {

template<typename T>
class AutoSerializer {
public:
    AutoSerializer(std::string_view path, std::string_view key_name_in_json, T& object_to_serialize)
        : _path{path}
        , _key_name_in_json{key_name_in_json}
        , _object_to_serialize{object_to_serialize}
    {
        Serialization::from_json(_object_to_serialize, _path);
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
