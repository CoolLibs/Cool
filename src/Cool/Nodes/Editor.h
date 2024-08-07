#pragma once
#include "IEditor.h"

namespace Cool {

class NodesEditor {
public:
    NodesEditor();

    auto imgui_windows(NodesConfig& nodes_cfg, NodesLibrary const& nodes_lib) -> bool { return _pimpl->imgui_windows(nodes_cfg, nodes_lib); }

    [[nodiscard]] auto graph() const -> NodesGraph const& { return _pimpl->graph(); }
    [[nodiscard]] auto graph() -> NodesGraph& { return _pimpl->graph(); }

    void for_each_selected_node(std::function<void(Node const&, NodeId const&)> const& callback) const { _pimpl->for_each_selected_node(callback); }

    [[nodiscard]] auto is_empty() const -> bool { return _pimpl->is_empty(); }

private:
    std::unique_ptr<INodesEditor> _pimpl; // We use the pimpl idiom to speed up compilation

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Pimpl", _pimpl)
        );
    }
};

} // namespace Cool