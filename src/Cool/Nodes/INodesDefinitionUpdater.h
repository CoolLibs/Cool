#pragma once

namespace Cool {

class INodesDefinitionUpdater {
public:
    INodesDefinitionUpdater()                                                  = default;
    virtual ~INodesDefinitionUpdater()                                         = default;
    INodesDefinitionUpdater(const INodesDefinitionUpdater&)                    = delete;
    INodesDefinitionUpdater(INodesDefinitionUpdater&&)                         = delete;
    auto operator=(const INodesDefinitionUpdater&) -> INodesDefinitionUpdater& = delete;
    auto operator=(INodesDefinitionUpdater&&) -> INodesDefinitionUpdater&      = delete;

    virtual void add_definition(std::filesystem::path const& path, std::filesystem::path const& root)    = 0;
    virtual void remove_definition(std::filesystem::path const& path, std::filesystem::path const& root) = 0;
};

}; // namespace Cool
