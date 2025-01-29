#include "JsonAutoSerializer.hpp"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/File/File.h"
#include "Cool/FileWatcher/FileWatcher.h"
#include "Cool/Log/ToUser.h"

namespace Cool {

JsonAutoSerializer::JsonAutoSerializer(
    std::filesystem::path const&               file_name,
    std::function<void(nlohmann::json const&)> from_json,
    std::function<void(nlohmann::json&)>       to_json,
    WantsToLogWarnings                         wants_to_log_warnings
)
    : _file_watcher{std::make_unique<FileWatcher>(Cool::Path::user_data() / file_name, FileWatcher_NoCallbacks() /*No callbacks because we want to ignore the initial error if the file doesn't exist*/)}
    , _from_json{std::move(from_json)}
    , _to_json{std::move(to_json)}
    , _wants_to_log_warnings{wants_to_log_warnings}
{
    // Load default user data, to get default values for fields that might not be serialized in UserData (e.g. new fields that have been added in a new version of the software that we open for the first time)
    load(Cool::Path::default_user_data() / file_name);
    // Overwrite values from DefaultUserData with the ones set in UserData
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

void JsonAutoSerializer::load(std::filesystem::path const& path)
{
    if (!Cool::File::exists(path))
        return; // Don't try to ifstream the file, it would log an error which we don't care about
    try
    {
        std::ifstream{path} >> _json; // NB: this clears the _json from its previous values. This means that when loading twice in a row (e.g. from UserDataDefault and then from UserData), only the last load will have its values stored in the _json. This is fine because we don't care about saving values from UserDataDefault into UserData if they are not used.
        _from_json(_json);
    }
    catch (std::exception const& e)
    {
        if (wants_to_log_warnings())
            Cool::Log::ToUser::warning("JSON", fmt::format("Corrupted file \"{}\":\n{}", Cool::File::weakly_canonical(path), e.what()));
    }
}

void JsonAutoSerializer::load()
{
    load(file_path());
}

void JsonAutoSerializer::update()
{
    _file_watcher->update(FileWatcher_Callbacks{
        .on_file_changed = [&](std::filesystem::path const& path) { load(path); },
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