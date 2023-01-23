#pragma once

#include "Cool/Log/MessageId.h"
#include "INodesDefinitionUpdater.h"
#include "folder_watcher/folder_watcher.hpp"

namespace Cool {

class NodesFolderWatcher {
public:
    explicit NodesFolderWatcher(std::filesystem::path folder_path, std::string extension);

    /// Returns true iff a node definition has been added, removed or updated in the library.
    auto update(INodesDefinitionUpdater&) -> bool;
    
    auto has_initialized(std::filesystem::path const& path, bool &has_changed) -> bool;
private:
    folder_watcher::FolderWatcher _folder_watcher{};
    std::string                   _extension;
    Cool::MessageId               _error_message_id{};
};

} // namespace Cool