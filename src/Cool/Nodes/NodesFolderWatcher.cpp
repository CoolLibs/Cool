#include "NodesFolderWatcher.h"
#include <utility>
#include "Cool/Log/ToUser.h"

namespace Cool {

NodesFolderWatcher::NodesFolderWatcher(std::filesystem::path folder_path, std::string extension)
    : _folder_watcher{std::move(folder_path)}
    , _extension{std::move(extension)}
{}

auto NodesFolderWatcher::update(INodesDefinitionUpdater& updater) -> bool
{
    bool has_changed = false;

    _folder_watcher.update({
        .on_file_added =
            [&](std::filesystem::path const& path)
        {
            // TODO(ML) refcator into function
            Cool::Log::ToUser::console().remove(_error_message_id);
            if (path.extension() != _extension)
                return;
            has_changed = true;

            updater.add_definition(path, _folder_watcher.get_folder_path());
        },

        .on_file_removed =
            [&](std::filesystem::path const& path)
        {
            has_changed = true;
            updater.remove_definition(path, _folder_watcher.get_folder_path());
        },

        .on_file_changed =
            [&](std::filesystem::path const& path)
        {
            has_changed = true;
            updater.remove_definition(path, _folder_watcher.get_folder_path());
            updater.add_definition(path, _folder_watcher.get_folder_path());
        },

        .on_invalid_folder_path =
            [&](std::filesystem::path const& path)
        {
            has_changed = true;
            Cool::Log::ToUser::console().send(
                _error_message_id,
                Message{
                    .category = "Nodes",
                    .message  = fmt::format("Nodes folder does not exist. '{}'", path),
                    .severity = MessageSeverity::Error,
                }
            );
        },
    });

    return has_changed;
}

}; // namespace Cool