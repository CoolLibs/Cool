#pragma once

#include <utility>
#include "Cool/ImGui/ImGuiExtras.h"
#include "NodeDefinition_Concept.h"
#include "NodesCategoryConfig.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace Cool {

namespace internal {
auto name_matches_filter(std::string const& _name, std::string const& filter) -> bool;
} // namespace internal

template<NodeDefinition_Concept NodeDefinition>
class NodesCategory {
public:
    NodesCategory(std::string name, std::filesystem::path const& path)
        : _name{std::move(name)}
        , _config{path}
    {}

    auto definitions() const -> auto const& { return _definitions; }
    auto definitions() -> auto& { return _definitions; }
    auto config() const -> auto const& { return _config; }
    auto config() -> auto& { return _config; }
    auto name() const -> auto const& { return _name; }

private:
    std::vector<NodeDefinition> _definitions{};
    NodesCategoryConfig         _config{};
    std::string                 _name{};
};

template<NodeDefinition_Concept NodeDefinition>
class NodesLibrary {
public:
    auto get_definition(std::string_view definition_name) const -> const NodeDefinition*
    {
        for (auto const& category : _categories)
        {
            const auto it = std::find_if(category._definitions.begin(), category._definitions.end(), [&](const NodeDefinition& def) {
                return def._name() == definition_name;
            });
            if (it != category._definitions.end())
                return &*it;
        }
    }
    auto get_definition(std::string_view definition_name) -> NodeDefinition*
    {
        for (auto& category : _categories)
        {
            const auto it = std::find_if(category.definitions().begin(), category.definitions().end(), [&](const NodeDefinition& def) {
                return def.name() == definition_name;
            });
            if (it != category.definitions().end())
                return &*it;
        }
        return nullptr;
    }

    auto imgui_nodes_menu(std::string const& nodes_filter, bool select_first, bool search_bar_focused, bool just_opened) const -> NodeDefinition const*
    {
        for (auto& category : _categories)
        {
            bool is_open = false;
            bool visible = true;
            if (!nodes_filter.empty())
            {
                visible = false;
                for (NodeDefinition const& def : category.definitions())
                {
                    if (internal::name_matches_filter(def.name(), nodes_filter))
                    {
                        is_open = true;
                        visible = true;
                    }
                }
            }

            if (!visible)
                continue;

            if (search_bar_focused || just_opened)
                ImGui::SetNextItemOpen(is_open);

            bool collapsing_header_clicked = ImGuiExtras::colored_collapsing_header(category.name(), category.config().get_color());

            category.config().imgui_popup();

            if (collapsing_header_clicked)
            {
                for (NodeDefinition const& def : category.definitions())
                {
                    if (!internal::name_matches_filter(def.name(), nodes_filter))
                    {
                        continue;
                    }

                    if (select_first || ImGui::Selectable(def.name().c_str()))
                    {
                        return &def;
                    }
                }
            }
        }
        return nullptr;
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
    mutable std::vector<NodesCategory<NodeDefinition>> _categories;
};

} // namespace Cool