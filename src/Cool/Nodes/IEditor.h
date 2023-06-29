#pragma once
#include "Graph.h"
#include "NodesConfig.h"
#include "NodesLibrary.h"

namespace Cool {

class INodesEditor {
public:
    INodesEditor()                                       = default;
    virtual ~INodesEditor()                              = default;
    INodesEditor(INodesEditor const&)                    = delete;
    auto operator=(INodesEditor const&) -> INodesEditor& = delete;
    INodesEditor(INodesEditor&&)                         = delete;
    auto operator=(INodesEditor&&) -> INodesEditor&      = delete;

    virtual auto imgui_windows(NodesConfig&, NodesLibrary const&) -> bool = 0;

    virtual void for_each_selected_node(std::function<void(Node const&)> const&) const = 0;

    [[nodiscard]] virtual auto graph() const -> Graph const& = 0;
    [[nodiscard]] virtual auto graph() -> Graph&             = 0;
};

} // namespace Cool