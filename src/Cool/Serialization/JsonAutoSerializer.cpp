#include "JsonAutoSerializer.hpp"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"

namespace Cool {

JsonAutoSerializer::JsonAutoSerializer(std::filesystem::path file_path)
    : _file_watcher{std::move(file_path), FileWatcher_Callbacks{}}
{
    load();
}

JsonAutoSerializer::~JsonAutoSerializer()
{
    save();
}

void JsonAutoSerializer::save()
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
        if (DebugOptions::log_internal_warnings())
            Cool::Log::ToUser::warning("JSON", fmt::format("Failed to save file \"{}\":\n{}", Cool::File::weakly_canonical(file_path()), e.what()));
    }
}

void JsonAutoSerializer::load()
{
    try
    {
        std::ifstream{file_path()} >> _json;
        _map.clear(); // New values might have been added to the json, so we clear the map to force us to go back to the json to read those values
    }
    catch (std::exception const& e)
    {
        if (DebugOptions::log_internal_warnings())
            Cool::Log::ToUser::warning("JSON", fmt::format("Corrupted file \"{}\":\n{}", Cool::File::weakly_canonical(file_path()), e.what()));
    }
}

} // namespace Cool