#pragma once

#include "NodeDefinition_Concept.h"
#include "imgui.h"

namespace Cool {

namespace internal {
auto name_matches_filter(std::string const& name, std::string const& filter) -> bool;
}

template<NodeDefinition_Concept NodeDefinition>
struct NodesCategory {
    std::string                 name{};
    std::vector<NodeDefinition> definitions{};
};

template<NodeDefinition_Concept NodeDefinition>
class NodesLibrary {
public:
    auto get_definition(std::string_view definition_name) const -> const NodeDefinition*
    {
        for (auto const& category : _categories)
        {
            const auto it = std::find_if(category.definitions.begin(), category.definitions.end(), [&](const NodeDefinition& def) {
                return def.name() == definition_name;
            });
            if (it != category.definitions.end())
                return &*it;
        }
    }
    auto get_definition(std::string_view definition_name) -> NodeDefinition*
    {
        for (auto& category : _categories)
        {
            const auto it = std::find_if(category.definitions.begin(), category.definitions.end(), [&](const NodeDefinition& def) {
                return def.name() == definition_name;
            });
            if (it != category.definitions.end())
                return &*it;
        }
        return nullptr;
    }

    auto imgui_nodes_menu(std::string const& nodes_filter, bool select_first, bool open_all_categories, bool menu_just_opened) const -> NodeDefinition const*
    {
        for (auto const& category : _categories)
        {
            bool is_open = false;
            bool is_visible = true;
            if (!nodes_filter.empty())
            {
                is_visible = false;
                for (NodeDefinition const& def : category.definitions)
                {
                    if (internal::name_matches_filter(def.name(), nodes_filter))
                    {
                        is_open = true;
                        is_visible = true;
                    }
                }
            }

            if (!is_visible)
                continue;

            if (open_all_categories || menu_just_opened)
                ImGui::SetNextItemOpen(is_open);

            if (ImGui::CollapsingHeader(category.name.c_str()))
            {
                for (NodeDefinition const& def : category.definitions)
                {
                    if (!internal::name_matches_filter(def.name(), nodes_filter))
                        continue;

                    if (select_first || ImGui::Selectable(def.name().c_str()))
                        return &def;
                }
            }
        }
        return nullptr;
    }

    void add_definition(NodeDefinition const& definition, std::string category_name)
    {
        if (category_name.empty())
            category_name = "Unnamed Category";

        for (auto& category : _categories)
        {
            if (category.name != category_name)
                continue;

            category.definitions.push_back(definition);
            return;
        }

        // Add new category if not found
        _categories.push_back({.name = category_name});
        _categories.back().definitions.push_back(definition);
    }
    void clear() { _categories.clear(); }

private:
    std::vector<NodesCategory<NodeDefinition>> _categories;
};

} // namespace Cool