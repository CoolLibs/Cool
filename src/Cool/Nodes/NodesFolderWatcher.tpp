#include "folder_watcher/src/FolderWatcher.hpp"
namespace Cool {

template<NodeDefinition_Concept NodeDefinition, NodeDefinitionParser<NodeDefinition> Parser>
inline auto NodesFolderWatcher::update(NodesLibrary<NodeDefinition>& library, Parser&& parse_definition) -> bool
{
    auto const callbacks = folder_watcher::Callbacks{
        .on_file_added   = [this, &library, &parse_definition](std::filesystem::path const& path) {
            auto const content = File::to_string(path);
            if (!content)
            {
                handle_error(path, content.error());
                return;
            }

            auto const definition = parse_definition(path, *content);
            if (!definition)
            {
                handle_error(path, definition.error());
                return;
            }

            auto const category_name = File::without_file_name(std::filesystem::relative(path, _folder_watcher.get_folder_path()));
            library.add_definition(*definition, category_name.string()); },
        .on_file_removed = [](std::filesystem::path const& path) {
            // faire fonction remove_definition
        },
        .on_file_changed = [](std::filesystem::path const& path) {
            // faire fonction remove_definition
        },
        .on_invalid_folder_path = [this](std::filesystem::path const& path) { handle_error(path, "The Nodes folder path is not valid."); },
    };

    _folder_watcher.update(callbacks);
    /*library.clear(); // TODO(JF) Don't do this, only update / remove the files when necessary
    _errors.clear(); // TODO(JF) Only clear when a definition is removed.

    for (auto const& entry : std::filesystem::recursive_directory_iterator{_folder_path})
    {
        if (!entry.is_regular_file() || entry.path().extension() != _extension)
            continue;

        auto const content = File::to_string(entry.path());
        if (!content)
        {
            handle_error(entry.path(), content.error());
            continue;
        }

        auto const definition = parse_definition(entry.path(), *content);
        if (!definition)
        {
            handle_error(entry.path(), definition.error());
            continue;
        }

        auto const category_name = File::without_file_name(std::filesystem::relative(entry.path(), _folder_watcher.get_path()));
        library.add_definition(*definition, category_name.string());
    }*/

    return true; // TODO(JF) Only return true if a file has changed
}

} // namespace Cool