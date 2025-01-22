#pragma once
#include <filesystem>
#include <map>
#include "Cool/FileWatcher/FileWatcher.h"
#include "JsonSerializableTypes.hpp"
#include "nlohmann/json.hpp"

namespace Cool {

/// To add a new serializable type, add it to JsonSerializableTypes.hpp
/// and then implement to_json() and from_json() for your type (cf. Time.hpp)
class JsonAutoSerializer {
public:
    explicit JsonAutoSerializer(std::filesystem::path file_path);
    ~JsonAutoSerializer();
    JsonAutoSerializer(JsonAutoSerializer const&)                = delete;
    JsonAutoSerializer& operator=(JsonAutoSerializer const&)     = delete;
    JsonAutoSerializer(JsonAutoSerializer&&) noexcept            = delete; // We could implement those one day if we need them
    JsonAutoSerializer& operator=(JsonAutoSerializer&&) noexcept = delete; // We could implement those one day if we need them

    template<typename T>
    auto get(std::string const& key, T default_value) -> T&
    {
        auto const it = _map.find(key);
        if (it != _map.end())
        {
            auto* val = std::get_if<T>(&it->second);
            if (val)
                return *val;
        }
        try
        {
            _map[key] = T{_json.at(key)};
            return std::get<T>(_map.at(key));
        }
        catch (...) // NOLINT(*empty-catch) Ignore the error, it is totally possible that the key is not in the JSON, or that it is not of the right type
        {}
        _map[key] = std::move(default_value);
        return std::get<T>(_map.at(key));
    }

    void save();
    void load();

    void update()
    {
        _file_watcher.update(FileWatcher_Callbacks{
            .on_file_changed = [&](auto&&) {
                load();
            },
        });
    }

private:
    auto file_path() const -> std::filesystem::path const& { return _file_watcher.path(); }

private:
    FileWatcher _file_watcher;

    nlohmann::json                               _json{};
    std::map<std::string, JsonSerializableTypes> _map{};
};

} // namespace Cool