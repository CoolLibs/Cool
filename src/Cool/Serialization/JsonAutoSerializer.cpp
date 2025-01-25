#include "JsonAutoSerializer.hpp"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/File/File.h"
#include "Cool/FileWatcher/FileWatcher.h"
#include "Cool/Log/ToUser.h"

namespace Cool {

JsonAutoSerializer::JsonAutoSerializer(
    std::filesystem::path                      file_path,
    std::function<void(nlohmann::json const&)> from_json,
    std::function<void(nlohmann::json&)>       to_json,
    WantsToLogWarnings                         wants_to_log_warnings
)
    : _file_watcher{std::make_unique<FileWatcher>(std::move(file_path), FileWatcher_NoCallbacks() /*No callbacks because we want to ignore the initial error if the file doesn't exist*/)}
    , _from_json{std::move(from_json)}
    , _to_json{std::move(to_json)}
    , _wants_to_log_warnings{wants_to_log_warnings}
{
    load();
}
JsonAutoSerializer::~JsonAutoSerializer()
{
    save();
}

void JsonAutoSerializer::save()
{
    try
    {
        _to_json(_json);
        std::ofstream{file_path()} << _json.dump(1);
    }
    catch (std::exception const& e)
    {
        if (wants_to_log_warnings())
            Cool::Log::ToUser::warning("JSON", fmt::format("Failed to save file \"{}\":\n{}", Cool::File::weakly_canonical(file_path()), e.what()));
    }
}

void JsonAutoSerializer::load()
{
    if (!Cool::File::exists(file_path()))
        return; // Don't try to ifstream the file, it would log an error which we don't care about
    try
    {
        std::ifstream{file_path()} >> _json;
        _from_json(_json);
    }
    catch (std::exception const& e)
    {
        if (wants_to_log_warnings())
            Cool::Log::ToUser::warning("JSON", fmt::format("Corrupted file \"{}\":\n{}", Cool::File::weakly_canonical(file_path()), e.what()));
    }
}

void JsonAutoSerializer::update()
{
    _file_watcher->update(FileWatcher_Callbacks{
        .on_file_changed = [&](auto&&) { load(); },
        .on_path_invalid = [&](std::filesystem::path const& path) {
            if (wants_to_log_warnings())
                Log::ToUser::warning("JSON", fmt::format("Can't open file \"{}\"", path));
        }
    });
}

auto JsonAutoSerializer::file_path() const -> std::filesystem::path const&
{
    return _file_watcher->path();
}

auto JsonAutoSerializer::wants_to_log_warnings() const -> bool
{
    switch (_wants_to_log_warnings)
    {
    case WantsToLogWarnings::Always:
        return true;
    case WantsToLogWarnings::Never:
        return false;
    default:
        return DebugOptions::log_internal_warnings();
    }
}

} // namespace Cool