#pragma once

#include "NodeDefinition.h"
#include "NodesLibrary.h"

namespace Cool {

template<NodeDefinition_Concept NodeDefT>
class GetNodeDefinition_Ref {
public:
    explicit GetNodeDefinition_Ref(NodesLibrary const& library)
        : _library{library}
    {}

    auto operator()(Cool::NodeDefinitionIdentifier const& id_names) const -> const NodeDefT*
    {
        auto const* const maybe_def = _library.get().get_definition(id_names);
        if (!maybe_def)
            return nullptr;
        return &maybe_def->downcast<NodeDefT>();
    }

private:
    std::reference_wrapper<NodesLibrary const> _library;
};

template<NodeDefinition_Concept NodeDefT>
class GetMutableNodeDefinition_Ref {
public:
    explicit GetMutableNodeDefinition_Ref(NodesLibrary& library)
        : _library{library}
    {}

    auto operator()(Cool::NodeDefinitionIdentifier const& id_names) -> NodeDefT*
    {
        auto* const maybe_def = _library.get().get_definition(id_names);
        if (!maybe_def)
            return nullptr;
        return &maybe_def->downcast<NodeDefT>();
    }

private:
    std::reference_wrapper<NodesLibrary> _library;
};

} // namespace Cool