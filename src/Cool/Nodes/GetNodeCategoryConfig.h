#pragma once
#include <optional>
#include "Cool/Nodes/NodesCategoryConfig.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "NodesLibrary.h"

namespace Cool {

class GetNodeCategoryConfig_Ref {
public:
    explicit GetNodeCategoryConfig_Ref(NodesLibrary const& library)
        : _library{library}
    {}

    auto operator()(std::string const& category_name) const -> std::optional<NodesCategoryConfig>
    {
        auto const* cat = _library.get().get_category(category_name);
        if (!cat)
            return std::nullopt;
        return cat->config();
    }

private:
    std::reference_wrapper<NodesLibrary const> _library;
};

} // namespace Cool