#pragma once

#include "NodeDefinition_Concept.h"
#include "NodesLibrary.h"

namespace Cool {

template<NodeDefinition_Concept NodeDefinition>
class GetNodeDefinition_Ref {
public:
    GetNodeDefinition_Ref(NodesLibrary<NodeDefinition> const& library)
        : _library{library}
    {}

    auto operator()(std::string_view definition_name) const -> const NodeDefinition*
    {
        return _library.get().get_definition(definition_name);
    }

private:
    std::reference_wrapper<const NodesLibrary<NodeDefinition>> _library;
};

} // namespace Cool