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

    template<NodeDefinition_Concept NodeDefinition, NodeDefinitionParser<NodeDefinition> Parser>
    void update(NodesLibrary<NodeDefinition>& library, Parser&& parse_definition)
    {
        if (!_has_changed)
            return;

        _has_changed = false;

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
    }

    void change_path(std::filesystem::path folder_path)
    {
        _folder_path = std::move(folder_path);
        _has_changed = true;
    }

private:
    // TODO(JF) Folder Watcher, or at least a file watcher for each current node to auto refresh it
    std::filesystem::path _folder_path;
    bool                  _has_changed{true}; // TODO(JF) Remove, use update of folder watcher instead
};

} // namespace Cool