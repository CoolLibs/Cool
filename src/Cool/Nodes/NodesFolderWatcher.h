#pragma once

#include <filesystem>
#include <map>
#include <utility>
#include "Cool/File/File.h"
#include "Cool/Log/Message.h"
#include "Cool/Log/MessageConsole.h"
#include "Cool/Log/MessageId.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "INodesDefinitionUpdater.h"
#include "folder_watcher/src/FolderWatcher.hpp"


namespace Cool {

class NodesFolderWatcher {
public:
    explicit NodesFolderWatcher(std::filesystem::path folder_path, std::string extension);

    /// Returns true iff a node definition has been added, removed or updated in the library.
    auto update(INodesDefinitionUpdater&) -> bool;

    void change_path(std::filesystem::path const&);

private:
    folder_watcher::FolderWatcher                    _folder_watcher{};
    std::string                                      _extension;
    folder_watcher::Callbacks                        _callbacks{};
    std::map<std::filesystem::path, Cool::MessageId> _errors{};
};

} // namespace Cool