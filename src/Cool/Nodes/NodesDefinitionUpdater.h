#pragma once

#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Nodes/NodeDefinitionIdentifier.h"
#include "Cool/Nodes/NodesCfg_Concept.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Graph.h"
#include "INodesDefinitionUpdater.h"
#include "folder_watcher/folder_watcher.hpp"

namespace Cool {

auto get_category_name(std::filesystem::path const& path, std::filesystem::path const& root) -> std::string;

template<NodeDefinition_Concept NodeDefinition>
using NodeDefinitionParser = std::function<tl::expected<NodeDefinition, std::string>(std::filesystem::path const&, std::string const&)>;

template<NodesCfg_Concept NodesConfig>
class NodesDefinitionUpdater : public INodesDefinitionUpdater {
public:
    NodesDefinitionUpdater(
        NodesConfig const&                                          config,
        GraphImpl&                                                  graph,
        NodesLibrary<typename NodesConfig::NodeDefinitionT>&        library,
        NodeDefinitionParser<typename NodesConfig::NodeDefinitionT> parse_definition,
        std::map<std::filesystem::path, Cool::MessageId>&           errors
    )
        : _config{config}
        , _graph{graph}
        , _library{library}
        , _parse_definition{parse_definition}
        , _errors{errors}
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

        auto const category_name = get_category_name(path, root);

        auto const category_folder = File::without_file_name(path).string();
        _library.add_definition(*definition, category_name, category_folder);

        {
            // Update all nodes that use that definition
            // auto lock = std::unique_lock{_graph.nodes().mutex()};
            for (auto& [_, node] : _graph.nodes())
            {
                if (node->category_name() == category_name && node->definition_name() == definition->name())
                    _config.update_node_with_new_definition(node, *definition, _graph);
            }
        }
    }

    void remove_definition(std::filesystem::path const& path, std::filesystem::path const& root) override
    {
        _library.remove_definition({
            .definition_name = File::file_name_without_extension(path).string(),
            .category_name   = get_category_name(path, root),
        });
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
    NodesConfig const&                                          _config;
    GraphImpl&                                                  _graph;
    NodesLibrary<typename NodesConfig::NodeDefinitionT>&        _library;
    NodeDefinitionParser<typename NodesConfig::NodeDefinitionT> _parse_definition;
    std::map<std::filesystem::path, Cool::MessageId>&           _errors;
};
}; // namespace Cool
