#pragma once

#include <utility>
#include "Cool/ImGui/ImGuiExtras.h"
#include "NodeDefinitionIdentifier.h"
#include "NodeDefinition_Concept.h"
#include "NodesCategoryConfig.h"
#include "imgui.h"

namespace Cool {

namespace internal {
auto name_matches_filter(std::string const& _name, std::string const& filter) -> bool;
} // namespace internal

template<NodeDefinition_Concept NodeDefinition>
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

private:
    std::vector<NodeDefinition> _definitions{};
    NodesCategoryConfig         _config;
    std::string                 _name{};
};

template<NodeDefinition_Concept NodeDefinition>
struct NodeDefinitionAndCategoryName {
    NodeDefinition def;
    std::string    category_name;
};

template<NodeDefinition_Concept NodeDefinition>
class NodesLibrary {
public:
    auto get_definition(Cool::NodeDefinitionIdentifier const& id_names) const -> NodeDefinition const* { return internal_get_definition(*this, id_names); }
    auto get_definition(Cool::NodeDefinitionIdentifier const& id_names) -> NodeDefinition* { return internal_get_definition(*this, id_names); }

    auto get_category(std::string const& category_name) const -> NodesCategory<NodeDefinition> const*
    {
        auto const it = std::find_if(_categories.begin(), _categories.end(), [&](NodesCategory<NodeDefinition> const& cat) {
            return cat.name() == category_name;
        });

        if (it != _categories.end())
            return &*it;
        return nullptr;
    }

    auto imgui_nodes_menu(std::string const& nodes_filter, bool select_first, bool open_all_categories, bool menu_just_opened) const -> std::optional<NodeDefinitionAndCategoryName<NodeDefinition>>
    {
        for (auto& category : _categories)
        {
            bool is_open    = false;
            bool is_visible = true;
            if (!nodes_filter.empty())
            {
                is_visible = false;
                for (NodeDefinition const& def : category.definitions())
                {
                    if (internal::name_matches_filter(def.name(), nodes_filter))
                    {
                        is_open    = true;
                        is_visible = true;
                    }
                }
            }

            if (!is_visible)
                continue;

            if (open_all_categories || menu_just_opened)
                ImGui::SetNextItemOpen(is_open);

            bool collapsing_header_clicked = ImGuiExtras::colored_collapsing_header(category.name(), category.config().get_color());

            category.config().imgui_popup();

            if (collapsing_header_clicked)
            {
                for (NodeDefinition const& def : category.definitions())
                {
                    if (!internal::name_matches_filter(def.name(), nodes_filter))
                        continue;

                    if (select_first || ImGui::Selectable(def.name().c_str()))
                    {
                        return NodeDefinitionAndCategoryName<NodeDefinition>{def, category.name()};
                    }
                }
            }
        }
        return std::nullopt;
    }

    void add_definition(NodeDefinition const& definition, std::string category_name, std::filesystem::path const& category_folder)
    {
        if (category_name.empty())
            category_name = "Unnamed Category";

        // Add definition to the corresponding category if it exists
        for (auto& category : _categories)
        {
            if (category.name() != category_name)
                continue;

            category.definitions().push_back(definition);
            return;
        }

        // Add new category if not found
        _categories.push_back(NodesCategory<NodeDefinition>{category_name, category_folder});
        _categories.back().definitions().push_back(definition);
    }
    void clear() { _categories.clear(); }

private:
    template<typename T>
    static auto internal_get_definition(T&& nodes_library, Cool::NodeDefinitionIdentifier const& id_names) -> NodeDefinition*
    {
        // Find category
        auto const* category = nodes_library.get_category(id_names.category_name);
        if (!category)
            return nullptr;

        // Find definition inside the category
        auto const it = std::find_if(category->definitions().begin(), category->definitions().end(), [&](NodeDefinition const& def) {
            return def.name() == id_names.definition_name;
        });
        if (it != category->definitions().end())
            return &*it;
        return nullptr;
    }

private:
    mutable std::vector<NodesCategory<NodeDefinition>>
        _categories;
};

} // namespace Cool