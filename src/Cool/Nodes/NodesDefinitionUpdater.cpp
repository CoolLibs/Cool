#include "NodesDefinitionUpdater.h"

namespace Cool {

static auto get_category_name(std::filesystem::path const& path, std::filesystem::path const& root) -> std::string
{
    std::string const category_name = File::without_file_name(std::filesystem::relative(path, root)).string();
    return category_name.empty() ? "Unnamed Category" : category_name;
}

void NodesDefinitionUpdater::add_definition(std::filesystem::path const& path, std::filesystem::path const& root)
{
    auto const content = File::to_string(path);
    if (!content)
    {
        handle_error(path, content.error());
        return;
    }

    auto const definition = _parse_definition(path, *content);
    if (!definition)
    {
        handle_error(path, definition.error());
        return;
    }

    auto const category_name = get_category_name(path, root);

    auto const category_folder = File::without_file_name(path).string();
    _library.add_definition(*definition, category_name, category_folder);

    {
        // Update all nodes that use that definition
        auto lock = std::unique_lock{_graph.nodes().mutex()};
        for (auto& [_, node] : _graph.nodes())
        {
            if (node.category_name() == category_name && node.definition_name() == definition->name())
                _config.update_node_with_new_definition(node, *definition, _graph);
        }
    }
}

void NodesDefinitionUpdater::remove_definition(std::filesystem::path const& path, std::filesystem::path const& root)
{
    _library.remove_definition({
        .definition_name = File::file_name_without_extension(path).string(),
        .category_name   = get_category_name(path, root),
    });
}

void NodesDefinitionUpdater::handle_error(std::filesystem::path const& definition_path, std::string const& message)
{
    Cool::Log::ToUser::console().send(
        _errors[definition_path], // This will create an error id if not already present in the map. This is what we want.
        Message{
            .category = "Nodes",
            .message  = fmt::format("Failed to read node from file {}:\n{}", definition_path, message),
            .severity = MessageSeverity::Error,
        }
    );
}

}; // namespace Cool
