#pragma once

#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Nodes/NodeDefinitionIdentifier.h"
#include "Cool/Nodes/NodeDefinition_Concept.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "INodesDefinitionUpdater.h"
#include "folder_watcher/folder_watcher.hpp"

namespace Cool {

template<NodeDefinition_Concept NodeDefinition>
using NodeDefinitionParser = std::function<tl::expected<NodeDefinition, std::string>(std::filesystem::path const&, std::string const&)>;

template<NodeDefinition_Concept NodeDefinition>
class NodesDefinitionUpdater : public INodesDefinitionUpdater {
public:
    NodesDefinitionUpdater(NodesLibrary<NodeDefinition>& library, NodeDefinitionParser<NodeDefinition> parse_definition)
        : _library(library), _parse_definition(parse_definition)
    {}

    void add_definition(std::filesystem::path const& path, std::filesystem::path const& root) override
    {
        auto const content = File::to_string(path);
        if (!content)
        {
            handle_error(path, content.error());
            return;
        }

        auto const definition = _parse_definition(path, *content);
        if (!definition)
        {
            handle_error(path, definition.error());
            return;
        }

        auto const category_name   = File::without_file_name(std::filesystem::relative(path, root)).string();
        auto const category_folder = File::without_file_name(path).string();
        _library.add_definition(*definition, category_name, category_folder);
    }

    void remove_definition(std::filesystem::path const& path, std::filesystem::path const& root) override
    {
        auto identifier = NodeDefinitionIdentifier{
            .definition_name = File::file_name_without_extension(path).string(),
            .category_name   = File::without_file_name(std::filesystem::relative(path, root)).string(),
        };
        _library.remove_definition(identifier);
    }

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
    NodesLibrary<NodeDefinition>&                    _library;
    NodeDefinitionParser<NodeDefinition>             _parse_definition;
    std::map<std::filesystem::path, Cool::MessageId> _errors{};
};
}; // namespace Cool
