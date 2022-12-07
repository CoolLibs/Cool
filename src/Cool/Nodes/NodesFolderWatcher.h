#pragma once

#include <filesystem>
#include <utility>
#include "Cool/File/File.h"
#include "Cool/Nodes/NodesLibrary.h"

namespace Cool {

// clang-format off
template<typename T, typename NodeDefinition>
concept NodeDefinitionParser = NodeDefinition_Concept<NodeDefinition> && requires(T func) {
    { func(std::filesystem::path{}, std::string{}) } -> std::same_as<tl::expected<NodeDefinition, std::string>>; 
};
// clang-format on

class NodesFolderWatcher {
public:
    explicit NodesFolderWatcher(std::filesystem::path folder_path)
        : _folder_path{std::move(folder_path)}
    {}

    /// Returns true iff a node definition has been added, removed or updated in the library.
    template<NodeDefinition_Concept NodeDefinition, NodeDefinitionParser<NodeDefinition> Parser>
    auto update(NodesLibrary<NodeDefinition>& library, Parser&& parse_definition) -> bool
    {
        if (!_has_changed)
            return false;

        _has_changed = false;
        library.clear(); // TODO(JF) Don't do this, only update / remove the files when necessary

        for (auto const& entry : std::filesystem::recursive_directory_iterator{_folder_path})
        {
            if (!entry.is_regular_file())
                continue;

            auto const content = File::to_string(entry.path());
            if (!content)
            {
                // TODO(JF) log and store the error
                continue;
            }

            auto const definition = parse_definition(entry.path(), *content);
            if (!definition)
            {
                // TODO(JF) log and store the error
                continue;
            }

            library.add_definition(*definition);
        }

        return true; // TODO(JF) Only return true if a file has changed
    }

    void change_path(std::filesystem::path folder_path)
    {
        _folder_path = std::move(folder_path);
        _has_changed = true;
    }

    void force_refresh() { _has_changed = true; } // TODO(JF) Remove

private:
    // TODO(JF) Folder Watcher, or at least a file watcher for each current node to auto refresh it
    std::filesystem::path _folder_path;
    bool                  _has_changed{true}; // TODO(JF) Remove, use update of folder watcher instead
};

} // namespace Cool