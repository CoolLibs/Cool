#pragma once

#include <utility>
#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Graph.h"
#include "INodesDefinitionUpdater.h"
#include "NodeDefinitionIdentifier.h"
#include "NodesConfig.h"
#include "NodesLibrary.h"
#include "folder_watcher/folder_watcher.hpp"

namespace Cool {

using NodeDefinitionParser = std::function<tl::expected<NodeDefinition, std::string>(std::filesystem::path const&, std::string const&)>;

class NodesDefinitionUpdater : public INodesDefinitionUpdater {
public:
    NodesDefinitionUpdater(
        NodesConfig const&                                config,
        Graph&                                            graph,
        NodesLibrary&                                     library,
        NodeDefinitionParser                              parse_definition,
        std::map<std::filesystem::path, Cool::MessageId>& errors
    )
        : _config{config}
        , _graph{graph}
        , _library{library}
        , _parse_definition{std::move(parse_definition)}
        , _errors{errors}
    {}

    void add_definition(std::filesystem::path const& path, std::filesystem::path const& root) override;
    void remove_definition(std::filesystem::path const& path, std::filesystem::path const& root) override;

private:
    void handle_error(std::filesystem::path const& definition_path, std::string const& message);

private:
    NodesConfig const&                                          _config;
    Graph&                                                      _graph;
    NodesLibrary&                                               _library;
    NodeDefinitionParser                                        _parse_definition;
    std::map<std::filesystem::path, Cool::MessageId>&           _errors;
};

}; // namespace Cool
