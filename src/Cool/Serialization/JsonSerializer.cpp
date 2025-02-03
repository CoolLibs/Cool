#include "JsonSerializer.hpp"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"

namespace Cool {

JsonSerializer::JsonSerializer(
    std::filesystem::path const&                      file_name,
    std::function<void(nlohmann::json const&)> const& from_json,
    std::function<void(nlohmann::json&)> const&       to_json,
    WantsToLogWarnings                                wants_to_log_warnings
)
    : _file_path{Cool::Path::user_data() / file_name}
    , _from_json{from_json}
    , _to_json{to_json}
    , _wants_to_log_warnings{wants_to_log_warnings}
{}

void JsonSerializer::save()
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

void JsonSerializer::load(std::filesystem::path const& path)
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

void JsonSerializer::load()
{
    load(file_path());
}

auto JsonSerializer::wants_to_log_warnings() const -> bool
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