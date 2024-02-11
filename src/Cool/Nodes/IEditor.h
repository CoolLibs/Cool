#pragma once
#include "NodesConfig.h"
#include "NodesGraph.h"
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

    virtual void for_each_selected_node(std::function<void(Node const&, NodeId const&)> const&) const = 0;

    [[nodiscard]] virtual auto graph() const -> NodesGraph const& = 0;
    [[nodiscard]] virtual auto graph() -> NodesGraph&             = 0;

    [[nodiscard]] virtual auto is_empty() const -> bool = 0;
};

} // namespace Cool