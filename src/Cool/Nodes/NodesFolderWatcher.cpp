#include "NodesFolderWatcher.h"
#include <utility>
#include "Cool/Log/ToUser.h"

namespace Cool {

NodesFolderWatcher::NodesFolderWatcher(std::filesystem::path folder_path, std::string extension)
    : _folder_watcher{std::move(folder_path)}
    , _extension{std::move(extension)}
{}

auto NodesFolderWatcher::update(
    INodesDefinitionUpdater&                                                updater,
    std::function<NodesCategoryConfig(std::filesystem::path const&)> const& make_category_config
) -> bool
{
    bool       has_changed                      = false;
    auto const clear_errors_and_check_extension = [&](std::filesystem::path const& path) {
        Cool::Log::ToUser::console().remove(_error_message_id);

        if (path.extension() != _extension)
            return false;

        Cool::Log::ToUser::console().remove(_node_parsing_errors[path]);
        has_changed = true;
        return true;
    };

    _folder_watcher.update({
        .on_file_added = [&](std::filesystem::path const& path) {
            if (!clear_errors_and_check_extension(path))
                return;

            updater.add_definition(path, _folder_watcher.get_folder_path(), make_category_config); },

        .on_file_removed = [&](std::filesystem::path const& path) {
            if (!clear_errors_and_check_extension(path))
                return;
            
            updater.remove_definition(path, _folder_watcher.get_folder_path()); },

        .on_file_changed = [&](std::filesystem::path const& path) {
            if (!clear_errors_and_check_extension(path))
                return;
            
            updater.remove_definition(path, _folder_watcher.get_folder_path());
            updater.add_definition(path, _folder_watcher.get_folder_path(), make_category_config); },

        .on_invalid_folder_path = [&](std::filesystem::path const& path) {
            has_changed = true;
            Cool::Log::ToUser::console().send(
                _error_message_id,
                Message{
                    .category = "Nodes",
                    .message  = fmt::format("Nodes folder does not exist. '{}'", path),
                    .severity = MessageSeverity::Error,
                }
            ); },
    });

    return has_changed;
}

}; // namespace Cool