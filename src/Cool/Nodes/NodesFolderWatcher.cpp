#include "NodesFolderWatcher.h"
#include <utility>
#include "Cool/Nodes/NodesFolderWatcher.h"

namespace Cool {

NodesFolderWatcher::NodesFolderWatcher(std::filesystem::path folder_path, std::string extension)
    : _folder_watcher{std::move(folder_path)}
    , _extension{std::move(extension)}
{
}

void NodesFolderWatcher::change_path(std::filesystem::path const& folder_path)
{
    _folder_watcher.set_folder_path(_callbacks, folder_path);
}

void NodesFolderWatcher::handle_error(std::filesystem::path const& definition_path, std::string const& message)
{
    Cool::Log::ToUser::console().send(
        _errors[definition_path], // This will create an error id if not already present in the map. This is what we want.
        Message{
            .category = "Nodes",
            .message  = fmt::format("Failed to read node from file {}:\n{}", definition_path, message),
            .severity = MessageSeverity::Error,
        }
    );
}

}; // namespace Cool