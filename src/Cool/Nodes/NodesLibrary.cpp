#include "NodesLibrary.h"
#include <Cool/String/String.h>
#include <imgui.h>
#include <wafl/wafl.hpp>
#include "Cool/Icons/Icons.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool {

static auto name_matches_filter(std::string const& name, std::string const& filter) -> bool
{
    return wafl::similarity_match({.input = filter, .reference = name}) >= wafl::Matches::Strongly;
}

auto NodesLibrary::get_category(std::string const& category_name) const -> NodesCategory const*
{
    auto const it = std::find_if(_categories.begin(), _categories.end(), [&](NodesCategory const& cat) {
        return cat.name() == category_name;
    });

    if (it != _categories.end())
        return &*it;
    return nullptr;
}

auto NodesLibrary::get_category(std::string const& category_name) -> NodesCategory*
{
    auto const it = std::find_if(_categories.begin(), _categories.end(), [&](NodesCategory const& cat) {
        return cat.name() == category_name;
    });

    if (it != _categories.end())
        return &*it;
    return nullptr;
}

static auto bob(NodesCategory& category, std::string const& nodes_filter, MaybeDisableNodeDefinition const& maybe_disable, bool select_first, bool open_all_categories, bool menu_just_opened)
    -> std::optional<NodeDefinitionAndCategoryName>
{
    ImGui::PushID(&category);
    auto const pop_automatically = sg::make_scope_guard([]() { ImGui::PopID(); });

    bool is_open    = false;
    bool is_visible = true;
    if (!nodes_filter.empty())
    {
        is_visible = false;
        for (NodeDefinition const& def : category.definitions())
        {
            if (name_matches_filter(def.name(), nodes_filter))
            {
                is_open    = true;
                is_visible = true;
            }
        }
    }

    if (!is_visible)
        return std::nullopt;

    if (open_all_categories || menu_just_opened)
        ImGui::SetNextItemOpen(is_open);

    ImGui::PushID(13452);
    bool const collapsing_header_clicked = ImGuiExtras::colored_collapsing_header(category.name(), category.config().color());
    ImGui::PopID();

    category.config().imgui_popup();

    auto const button_size = 50.f;

    if (collapsing_header_clicked)
    {
        // ImGui::BeginHorizontal(&category);
        bool first = true;
        ImGuiExtras::fill_layout("##nodes", button_size, [&](std::function<void()> const& next_item) {
            for (NodeDefinition const& def : category.definitions())
            {
                if (!name_matches_filter(def.name(), nodes_filter))
                    continue;

                if (!first && !(button_size > ImGui::GetContentRegionAvail().x))
                    ImGui::SameLine();
                first = false;

                auto selected_definition = std::optional<NodeDefinitionAndCategoryName>{};
                Cool::ImGuiExtras::disabled_if(maybe_disable(def, category), [&]() {
                    bool b = ImGuiExtras::button_with_icon(Cool::Icons::close_button().imgui_texture_id(), ImVec4(1., 1., 1., 1.), ImVec4(0., 0., 0, 1), button_size, button_size);
                    b |= select_first;
                    if (b)
                        selected_definition = NodeDefinitionAndCategoryName{def, category.name()};
                });

                // if (selected_definition.has_value()) // TODO
                //     return selected_definition;

                next_item();
            }
        });
        // ImGui::EndHorizontal();
    }
    return std::nullopt;
}

auto NodesLibrary::imgui_nodes_menu(std::string const& nodes_filter, MaybeDisableNodeDefinition const& maybe_disable, bool select_first, bool open_all_categories, bool menu_just_opened) const -> std::optional<NodeDefinitionAndCategoryName>
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::BeginGroup();
    ImGui::PushFont(Font::window_title());
    ImGui::SeparatorText("Images");
    ImGui::PopFont();

    ImGui::BeginChild("1", ImVec2(300, 260), ImGuiChildFlags_None, window_flags);
    for (size_t i = 0; i < 3; ++i)
    {
        auto bob2 = bob(_categories[i], nodes_filter, maybe_disable, select_first, open_all_categories, menu_just_opened);
        if (bob2)
            return bob2;
    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::PushFont(Font::window_title());
    ImGui::SeparatorText("Greyscale");
    ImGui::PopFont();

    ImGui::BeginChild("2", ImVec2(300, 260), ImGuiChildFlags_None, window_flags);
    for (size_t i = 3; i < 6; ++i)
    {
        auto bob2 = bob(_categories[i], nodes_filter, maybe_disable, select_first, open_all_categories, menu_just_opened);
        if (bob2)
            return bob2;
    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::PushFont(Font::window_title());
    ImGui::SeparatorText("Shapes");
    ImGui::PopFont();

    ImGui::BeginChild("3", ImVec2(300, 260), ImGuiChildFlags_None, window_flags);
    for (size_t i = 6; i < 9; ++i)
    {
        auto bob2 = bob(_categories[i], nodes_filter, maybe_disable, select_first, open_all_categories, menu_just_opened);
        if (bob2)
            return bob2;
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    return std::nullopt;
}

void NodesLibrary::add_definition(
    NodeDefinition const&                 definition,
    std::string const&                    category_name,
    std::function<NodesCategory()> const& make_category // We take a function because we want to delay the creation of the category until we are sure we need to create one, which is rare (and otherwise would cause warning when two folders correspond to the same category, but only one of them contains the _category_config.json, which happens when reading nodes from both the app's root folder and the user-data folder)
)
{
    // Add definition to the corresponding category if it exists
    for (auto& category : _categories)
    {
        if (category.name() != category_name)
            continue;

        category.definitions().push_back(definition);
        category.sort();
        return;
    }

    // Add new category if not found
    _categories.push_back(make_category());
    _categories.back().definitions().push_back(definition);
    std::sort(
        _categories.begin(), _categories.end(),
        [](NodesCategory const& c1, NodesCategory const& c2) {
            return c1.order() < c2.order();
        }
    );
}

void NodesLibrary::remove_definition(NodeDefinitionIdentifier const& identifier)
{
    auto* category = get_category(identifier.category_name);
    if (!category)
        return;

    // Remove the node from the definitions
    std::erase_if(category->definitions(), [&](NodeDefinition const& def) { return def.name() == identifier.definition_name; });

    // Remove the category from _categories if is now empty.
    if (category->definitions().empty())
    {
        std::erase_if(_categories, [&](NodesCategory const& category) {
            return category.name() == identifier.category_name;
        });
    }
}

auto NodesLibrary::is_empty() const -> bool
{
    return _categories.empty();
}

} // namespace Cool