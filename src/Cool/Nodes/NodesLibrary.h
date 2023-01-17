#pragma once

#include <cereal/archives/json.hpp>
#include <filesystem>
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/as_json.h"
#include "Cool/StrongTypes/Color.h"
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

    auto imgui_nodes_menu(std::string const& nodes_filter, bool select_first, bool search_bar_focused, bool just_opened) const -> NodeDefinition const*
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
            color*= 255.;
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(ImColor(color.x, color.y, color.z)));
            color*= 1.5;
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
                        return &def;
                    }
                }
            }
            ImGui::PopStyleColor(3);
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