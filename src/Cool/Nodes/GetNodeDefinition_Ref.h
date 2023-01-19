#pragma once

#include "NodeDefinition_Concept.h"
#include "NodesLibrary.h"

namespace Cool {

template<NodeDefinition_Concept NodeDefinition>
class GetNodeDefinition_Ref {
public:
    GetNodeDefinition_Ref(NodesLibrary<NodeDefinition>& library)
        : _library{library}
    {}

    auto operator()(Cool::NodeDefinitionIdentifier const& id_names) const -> const NodeDefinition*
    {
        return _library.get().get_definition(id_names);
    }
    auto operator()(Cool::NodeDefinitionIdentifier const& id_names) -> NodeDefinition*
    {
        return _library.get().get_definition(id_names);
    }

private:
    std::reference_wrapper<NodesLibrary<NodeDefinition>> _library;
};

} // namespace Cool