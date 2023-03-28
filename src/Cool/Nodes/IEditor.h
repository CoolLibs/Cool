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

    virtual auto imgui_window(NodesConfig const&, NodesLibrary const&) -> bool = 0;

    [[nodiscard]] virtual auto graph() const -> Graph const& = 0;
    [[nodiscard]] virtual auto graph() -> Graph&             = 0;
};

} // namespace Cool