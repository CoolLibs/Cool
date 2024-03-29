#include "NodesLibrary.h"
#include <Cool/String/String.h>
#include <wafl/wafl.hpp>
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool {

namespace internal {
auto name_matches_filter(std::string const& name, std::string const& filter) -> bool
{
    return wafl::similarity_match({.input = filter, .reference = name}) >= wafl::Matches::Strongly;
}
} // namespace internal

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

auto NodesLibrary::imgui_nodes_menu(std::string const& nodes_filter, MaybeDisableNodeDefinition const& maybe_disable, bool select_first, bool open_all_categories, bool menu_just_opened) const -> std::optional<NodeDefinitionAndCategoryName>
{
    for (auto& category : _categories)
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

        ImGui::PushID(13452);
        bool const collapsing_header_clicked = ImGuiExtras::colored_collapsing_header(category.name(), category.config().color());
        ImGui::PopID();

        category.config().imgui_popup();

        if (collapsing_header_clicked)
        {
            for (NodeDefinition const& def : category.definitions())
            {
                if (!internal::name_matches_filter(def.name(), nodes_filter))
                    continue;

                auto selected_definition = std::optional<NodeDefinitionAndCategoryName>{};
                Cool::ImGuiExtras::disabled_if(maybe_disable(def, category), [&]() {
                    if (select_first || ImGui::Selectable(def.name().c_str()))
                        selected_definition = NodeDefinitionAndCategoryName{def, category.name()};
                });

                if (selected_definition.has_value())
                    return selected_definition;
            }
        }
    }
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