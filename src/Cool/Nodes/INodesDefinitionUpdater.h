#pragma once
#include "NodesCategoryConfig.h"

namespace Cool {

class INodesDefinitionUpdater {
public:
    INodesDefinitionUpdater()                                                  = default;
    virtual ~INodesDefinitionUpdater()                                         = default;
    INodesDefinitionUpdater(const INodesDefinitionUpdater&)                    = delete;
    INodesDefinitionUpdater(INodesDefinitionUpdater&&)                         = delete;
    auto operator=(const INodesDefinitionUpdater&) -> INodesDefinitionUpdater& = delete;
    auto operator=(INodesDefinitionUpdater&&) -> INodesDefinitionUpdater&      = delete;

    virtual void add_definition(
        std::filesystem::path const& path, std::filesystem::path const& root,
        std::function<NodesCategoryConfig(std::filesystem::path const&)> const& make_category_config
    )                                                                                                                  = 0;
    virtual void               remove_definition(std::filesystem::path const& path, std::filesystem::path const& root) = 0;
    [[nodiscard]] virtual auto library_is_empty() const -> bool                                                        = 0;
};

}; // namespace Cool
