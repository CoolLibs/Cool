#include "NodesFolderWatcher.h"
#include <utility>

namespace Cool {

NodesFolderWatcher::NodesFolderWatcher(std::filesystem::path folder_path, std::string extension)
    : _folder_watcher{std::move(folder_path)}
    , _extension{std::move(extension)}
{
}

auto NodesFolderWatcher::update(INodesDefinitionUpdater& updater) -> bool
{
    bool       has_changed = false;
    auto const callbacks   = folder_watcher::Callbacks{
          .on_file_added = [this, &updater, &has_changed](std::filesystem::path const& path) {
            if (path.extension() != _extension)
                return;

            has_changed = true;
            updater.add_definition(path, _folder_watcher.get_folder_path()); },

          .on_file_removed = [this, &updater, &has_changed](std::filesystem::path const& path) {
            has_changed = true;
            updater.remove_definition(path, _folder_watcher.get_folder_path()); },

          .on_file_changed = [this, &updater, &has_changed](std::filesystem::path const& path) {
            has_changed = true;
            updater.remove_definition(path, _folder_watcher.get_folder_path());
            updater.add_definition(path, _folder_watcher.get_folder_path()); },

          .on_invalid_folder_path = [this, &has_changed](std::filesystem::path const& path) {
            has_changed = true;
            Cool::Log::ToUser::console().send(
                _errors[path], // This will create an error id if not already present in the map. This is what we want.
                Message{
                    .category = "Nodes",
                    .message  = fmt::format("Nodes folder path does not exists. Checked : {}:\n", path),
                    .severity = MessageSeverity::Error,
                }
            ); },
    };

    _folder_watcher.update(callbacks);

    return has_changed;
}

void NodesFolderWatcher::change_path(std::filesystem::path const& folder_path)
{
    _folder_watcher.set_folder_path(_callbacks, folder_path);
}

}; // namespace Cool