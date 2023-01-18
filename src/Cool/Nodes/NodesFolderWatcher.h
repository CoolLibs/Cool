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
#include "folder_watcher/src/FolderWatcher.hpp"

namespace Cool {

// clang-format off
template<typename T, typename NodeDefinition>
concept NodeDefinitionParser = NodeDefinition_Concept<NodeDefinition> && requires(T func) {
    { func(std::filesystem::path{}, std::string{}) } -> std::same_as<tl::expected<NodeDefinition, std::string>>; 
};
// clang-format on

class NodesFolderWatcher {
public:
    explicit NodesFolderWatcher(std::filesystem::path folder_path, std::string extension);

    /// Returns true iff a node definition has been added, removed or updated in the library.
    template<NodeDefinition_Concept NodeDefinition, NodeDefinitionParser<NodeDefinition> Parser>
    auto update(NodesLibrary<NodeDefinition>& library, Parser&& parse_definition) -> bool;

    void change_path(std::filesystem::path folder_path);

    void force_refresh() { _has_changed = true; } // TODO(JF) Remove

private:
    void handle_error(std::filesystem::path const& definition_path, std::string const& message);

private:
    // TODO(JF) Folder Watcher, or at least a file watcher for each current node to auto refresh it
    std::filesystem::path                            _folder_path;
    std::string                                      _extension;
    bool                                             _has_changed{true}; // TODO(JF) Remove, use update of folder watcher instead
    std::map<std::filesystem::path, Cool::MessageId> _errors{};
};

} // namespace Cool

#include "NodesFolderWatcher.tpp"