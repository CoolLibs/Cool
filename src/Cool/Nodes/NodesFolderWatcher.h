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

namespace Cool {

// clang-format off
template<typename T, typename NodeDefinition>
concept NodeDefinitionParser = NodeDefinition_Concept<NodeDefinition> && requires(T func) {
    { func(std::filesystem::path{}, std::string{}) } -> std::same_as<tl::expected<NodeDefinition, std::string>>; 
};
// clang-format on

class NodesFolderWatcher {
public:
    explicit NodesFolderWatcher(std::filesystem::path folder_path, std::string extension)
        : _folder_path{std::move(folder_path)}
        , _extension{std::move(extension)}
    {}

    /// Returns true iff a node definition has been added, removed or updated in the library.
    template<NodeDefinition_Concept NodeDefinition, NodeDefinitionParser<NodeDefinition> Parser>
    auto update(NodesLibrary<NodeDefinition>& library, Parser&& parse_definition) -> bool
    {
        if (!_has_changed)
            return false;

        _has_changed = false;
        library.clear(); // TODO(JF) Don't do this, only update / remove the files when necessary
        _errors.clear(); // TODO(JF) Only clear when a definition is removed.

        for (auto const& entry : std::filesystem::recursive_directory_iterator{_folder_path})
        {
            if (!entry.is_regular_file() || entry.path().extension() != _extension)
                continue;

            auto const content = File::to_string(entry.path());
            if (!content)
            {
                handle_error(entry.path(), content.error());
                continue;
            }

            auto const definition = parse_definition(entry.path(), *content);
            if (!definition)
            {
                handle_error(entry.path(), definition.error());
                continue;
            }

            auto const category_name = File::without_file_name(std::filesystem::relative(entry.path(), _folder_path));
            library.add_definition(*definition, category_name.string());
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
    void handle_error(std::filesystem::path const& definition_path, std::string const& message)
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

private:
    // TODO(JF) Folder Watcher, or at least a file watcher for each current node to auto refresh it
    std::filesystem::path                            _folder_path;
    std::string                                      _extension;
    bool                                             _has_changed{true}; // TODO(JF) Remove, use update of folder watcher instead
    std::map<std::filesystem::path, Cool::MessageId> _errors{};
};

} // namespace Cool