#pragma once
#include <filesystem>
#include <map>
#include "Cool/File/File.h"
#include "Cool/FileWatcher/FileWatcher.h"
#include "Cool/Log/ToUser.h"
#include "nlohmann/json.hpp"

namespace Cool {

namespace internal {
auto wants_to_log_internal_warnings() -> bool; // Needs to be implemented in the .cpp, otherwise there is a cyclic include between DebugOptions and JsonAutoSerializer
}

/// To add a new serializable type you need to implement to_json() and from_json() for your type (cf. Time.hpp)
template<typename... Ts>
class JsonAutoSerializer {
public:
    explicit JsonAutoSerializer(std::filesystem::path file_path, bool always_show_warnings = false /*this is for Cool::DebugOption, so it can tell the JSON to skip checking for DebugOptions to know if it needs to log warnings or not. Otherwise this creates a deadlock if the deserialization of DebugOption's json when it tries to log a warning (when the file is corrupted)*/)
        : _file_watcher{std::move(file_path), FileWatcher_NoCallbacks()}
        , _always_show_warnings{always_show_warnings}
    {
        load();
    }
    ~JsonAutoSerializer()
    {
        save();
    }
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

    void save()
    {
        // Put all of the values of the map into the json before serializing the json
        for (auto const& [key, value] : _map)
        {
            std::visit(
                [&](auto&& value) {
                    nlohmann::json j = value;
                    _json[key]       = j;
                },
                value
            );
        }
        try
        {
            std::ofstream{file_path()} << _json.dump(1);
        }
        catch (std::exception const& e)
        {
            if (_always_show_warnings || internal::wants_to_log_internal_warnings())
                Cool::Log::ToUser::warning("JSON", fmt::format("Failed to save file \"{}\":\n{}", Cool::File::weakly_canonical(file_path()), e.what()));
        }
    }

    void load()
    {
        if (!Cool::File::exists(file_path()))
            return; // Don't try to ifstream the file, it would log an error which we don't care about
        try
        {
            std::ifstream{file_path()} >> _json;
            _map.clear(); // New values might have been added to the json, so we clear the map to force us to go back to the json to read those values
        }
        catch (std::exception const& e)
        {
            if (_always_show_warnings || internal::wants_to_log_internal_warnings())
                Cool::Log::ToUser::warning("JSON", fmt::format("Corrupted file \"{}\":\n{}", Cool::File::weakly_canonical(file_path()), e.what()));
        }
    }

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

    nlohmann::json                             _json{};
    std::map<std::string, std::variant<Ts...>> _map{};

    bool _always_show_warnings{false};
};

} // namespace Cool