#pragma once

#include <utility>
#include <vector>
#include "Cool/ImGui/ImGuiExtras.h"
#include "NodeDefinition.h"
#include "NodeDefinitionIdentifier.h"
#include "NodesCategoryConfig.h"
#include "imgui.h"
#include "scope_guard/scope_guard.hpp"

namespace Cool {

namespace internal {
auto name_matches_filter(std::string const& _name, std::string const& filter) -> bool;
} // namespace internal

class NodesCategory {
public:
    NodesCategory(std::string name, std::filesystem::path const& path)
        : _config{path}
        , _name{std::move(name)}
    {}

    auto definitions() const -> auto const& { return _definitions; }
    auto definitions() -> auto& { return _definitions; }
    auto config() const -> auto const& { return _config; }
    auto config() -> auto& { return _config; }
    auto name() const -> auto const& { return _name; }
    void sort()
    {
        std::sort(_definitions.begin(), _definitions.end(), [](NodeDefinition const& d1, NodeDefinition const& d2)
                  { return d1.name() < d2.name(); });
    }

private:
    std::vector<NodeDefinition> _definitions{};
    NodesCategoryConfig         _config;
    std::string                 _name{};
};

struct NodeDefinitionAndCategoryName {
    NodeDefinition def;
    std::string    category_name;
};

class NodesLibrary {
public:
    auto get_definition(Cool::NodeDefinitionIdentifier const& id_names) const -> NodeDefinition const* { return internal_get_definition(*this, id_names); }
    auto get_definition(Cool::NodeDefinitionIdentifier const& id_names) -> NodeDefinition* { return internal_get_definition(*this, id_names); }

    auto get_category(std::string const& category_name) const -> NodesCategory const*;
    auto get_category(std::string const& category_name) -> NodesCategory*;

    auto imgui_nodes_menu(std::string const& nodes_filter, bool select_first, bool open_all_categories, bool menu_just_opened) const -> std::optional<NodeDefinitionAndCategoryName>;

    void add_definition(NodeDefinition const& definition, std::string category_name, std::filesystem::path const& category_folder);

    void remove_definition(NodeDefinitionIdentifier const& identifier);

private:
    template<typename T>
    static auto internal_get_definition(T&& nodes_library, Cool::NodeDefinitionIdentifier const& id_names) -> NodeDefinition*
    {
        for (auto&& category : nodes_library._categories)
        {
            if (category.name() != id_names.category_name)
                continue;

            auto const it = std::find_if(category.definitions().begin(), category.definitions().end(), [&](NodeDefinition const& def)
                                         { return def.name() == id_names.definition_name; });

            if (it != category.definitions().end())
                return &*it;
        }
        return nullptr;
    }

private:
    mutable std::vector<NodesCategory> _categories;
};

} // namespace Cool