#pragma once

#include <cereal/archives/json.hpp>
#include <filesystem>
#include <string>
#include "Cool/Nodes/NodeDefinitionIdentifier.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/as_json.h"
#include "Cool/StrongTypes/Color.h"
#include "NodeDefinitionIdentifier.h"
#include "NodeDefinition_Concept.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace Cool {

namespace internal {
auto name_matches_filter(std::string const& name, std::string const& filter) -> bool;
}

struct NodesCategoryConfig {
    Cool::Color color;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Color", color)
        ); // serialize things by passing them to the archive
    }
};

template<NodeDefinition_Concept NodeDefinition>
struct NodesCategory {
    std::string                 name{};
    std::vector<NodeDefinition> definitions{};
    NodesCategoryConfig         config{};
};

template<NodeDefinition_Concept NodeDefinition>
struct NodeCategoryIdentifier {
    NodeDefinition def;
    std::string    category_name;
};

template<NodeDefinition_Concept NodeDefinition>
class NodesLibrary {
public:
    // need to get the category name
    auto get_definition(Cool::NodeDefinitionIdentifier const& id_names) const -> const NodeDefinition*
    {
        for (auto const& category : _categories)
        {
            if (category.name != id_names.category_name)
                continue;

            const auto it = std::find_if(category.definitions.begin(), category.definitions.end(), [&](const NodeDefinition& def) {
                return def.name() == id_names.definition_name;
            });

            if (it != category.definitions.end())
                return &*it;
        }
    }

    auto get_definition(Cool::NodeDefinitionIdentifier const& id_names) -> NodeDefinition*
    {
        for (auto& category : _categories)
        {
            if (category.name != id_names.category_name)
                continue;

            const auto it = std::find_if(category.definitions.begin(), category.definitions.end(), [&](const NodeDefinition& def) {
                return def.name() == id_names.definition_name;
            });
            if (it != category.definitions.end())
                return &*it;
        }
        return nullptr;
    }

    // just need the category name ?
    auto get_category(std::string category_name) const -> const NodesCategory<NodeDefinition>*
    {
        const auto it = std::find_if(_categories.begin(), _categories.end(), [&](const NodesCategory<NodeDefinition>& cat) { return cat.name == category_name; });
        if (it != _categories.end())
            return &*it;
        return nullptr;
    }

    auto imgui_nodes_menu(std::string const& nodes_filter, bool select_first, bool search_bar_focused, bool just_opened) const -> std::optional<NodeCategoryIdentifier<NodeDefinition>>
    {
        for (auto const& category : _categories)
        {
            bool is_open = false;
            bool visible = true;
            if (!nodes_filter.empty())
            {
                visible = false;
                for (NodeDefinition const& def : category.definitions)
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

            glm::vec3 color = category.config.color.as_sRGB();
            color *= 255.;
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(ImColor(color.x, color.y, color.z)));
            color *= 1.5;
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(ImColor(color.x, color.y, color.z)));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(ImColor(color.x, color.y, color.z)));
            auto const b = ImGui::CollapsingHeader(category.name.c_str());
            if (b)
            {
                for (NodeDefinition const& def : category.definitions)
                {
                    if (!internal::name_matches_filter(def.name(), nodes_filter))
                    {
                        continue;
                    }

                    if (select_first || ImGui::Selectable(def.name().c_str()))
                    {
                        ImGui::PopStyleColor(3);
                        return NodeCategoryIdentifier<NodeDefinition>{def, category.name};
                    }
                }
            }
            ImGui::PopStyleColor(3);
        }
        return std::nullopt;
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

        std::filesystem::path const url = Cool::Path::root() / "Nodes" / category_name / "category_config.json";

        if (std::filesystem::exists(url))
        {
            // std::filesystem::remove(url);
            Serialization::from_json(_categories.back().config.color, url);
        }
        else
        {
            auto color = Cool::Color::from_srgb(glm::vec3(1, 0, 0));
            Serialization::to_json(color, url, "Color");
        }
        _categories.back().definitions.push_back(definition);
    }
    void clear() { _categories.clear(); }

private:
    std::vector<NodesCategory<NodeDefinition>> _categories;
};

} // namespace Cool