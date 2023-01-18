#pragma once

#include <algorithm>
#include <filesystem>
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/as_json.h"
#include "Cool/StrongTypes/Color.h"
#include "Cool/path/path.h"
#include "NodeDefinition_Concept.h"
#include "NodesCategoryConfig.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace Cool {

namespace internal {
auto name_matches_filter(std::string const& _name, std::string const& filter) -> bool;
auto popup(Cool::Color& color) -> bool;
} // namespace internal

template<NodeDefinition_Concept NodeDefinition>
struct NodesCategory {
    NodesCategory(std::string const& name, std::filesystem::path const& path)
        : _name{name}
        , _config{path}
    {
    }

    std::vector<NodeDefinition> _definitions{};
    NodesCategoryConfig         _config{};
    std::string                 _name{};
    auto                        display_collapsing_header() -> bool
    {
        auto const normal_color = this->_config.get_color().as_sRGB();
        auto const bright_color = glm::vec3(normal_color.x * 1.3, normal_color.y * 1.3, normal_color.z * 1.3);
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(ImColor(normal_color.x, normal_color.y, normal_color.z, 1.)));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(ImColor(bright_color.x, bright_color.y, bright_color.z, 1.)));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(ImColor(bright_color.x, bright_color.y, bright_color.z, 1.)));

        auto const b = ImGui::CollapsingHeader(this->_name.c_str());
        ImGui::PopStyleColor(3);
        return b;
    }
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
            const auto it = std::find_if(category._definitions.begin(), category._definitions.end(), [&](const NodeDefinition& def) {
                return def.name() == definition_name;
            });
            if (it != category._definitions.end())
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
                for (NodeDefinition const& def : category._definitions)
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

            bool collapsing_header_clicked = category.display_collapsing_header();

            category._config.imgui_popup();

            if (collapsing_header_clicked)
            {
                for (NodeDefinition const& def : category._definitions)
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
            if (category._name != category_name)
                continue;

            category._definitions.push_back(definition);
            return;
        }

        // Add new category if not found
        _categories.push_back(NodesCategory<NodeDefinition>{category_name, category_folder});
        _categories.back()._definitions.push_back(definition);
    }
    void clear() { _categories.clear(); }

private:
    mutable std::vector<NodesCategory<NodeDefinition>> _categories;
};

} // namespace Cool