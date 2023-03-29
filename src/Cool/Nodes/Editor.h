#pragma once
#include "IEditor.h"

namespace Cool {

class NodesEditor {
public:
    NodesEditor();

    auto imgui_window(NodesConfig const& nodes_cfg, NodesLibrary const& nodes_lib) -> bool { return _pimpl->imgui_window(nodes_cfg, nodes_lib); }

    [[nodiscard]] auto graph() const -> Graph const& { return _pimpl->graph(); }
    [[nodiscard]] auto graph() -> Graph& { return _pimpl->graph(); }

private:
    std::unique_ptr<INodesEditor> _pimpl; // We use the pimpl idiom to speed up compilation

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
#if COOL_SERIALIZATION
        archive(
            cereal::make_nvp("Pimpl", _pimpl)
        );
#else
        (void)archive;
#endif
    }
};

} // namespace Cool