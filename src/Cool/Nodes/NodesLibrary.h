#pragma once

#include "NodeDefinition_Concept.h"

namespace Cool {

template<NodeDefinition_Concept NodeDefinition>
class NodesLibrary {
public:
    auto get_definition(std::string_view definition_name) const -> const NodeDefinition*
    {
        const auto it = std::find_if(_definitions.begin(), _definitions.end(), [&](const NodeDefinition& def) {
            return def.name() == definition_name;
        });
        if (it == _definitions.end())
            return nullptr;

        return &*it;
    }

    auto imgui_nodes_menu() const -> NodeDefinition const*
    {
        for (NodeDefinition const& def : _definitions)
        {
            if (ImGui::Selectable(def.name().c_str()))
                return &def;
        }
        return nullptr;
    }

    void clear()
    {
        _definitions.clear();
    }

    void add_nodes_from(std::filesystem::path)
    {
        // TODO(JF)
    }

    void add_definition(NodeDefinition const& definition) { _definitions.push_back(definition); }
    void add_definition(NodeDefinition&& definition) { _definitions.emplace_back(std::move(definition)); }

private:
    std::vector<NodeDefinition> _definitions;
    // TODO(JF) Folder Watcher, or at least a file watcher for each current node to auto refresh it
};

} // namespace Cool