#include "Cool/Nodes/NodeDefinitionIdentifier.h"
#include "NodeDefinitionIdentifier.h"
#include "folder_watcher/src/FolderWatcher.hpp"

namespace Cool {

template<NodeDefinition_Concept NodeDefinition, NodeDefinitionParser<NodeDefinition> Parser>
auto NodesFolderWatcher::update(NodesLibrary<NodeDefinition>& library, Parser&& parse_definition) -> bool
{
    auto const callbacks = folder_watcher::Callbacks{
        .on_file_added = [this, &library, &parse_definition](std::filesystem::path const& path) {
            if (path.extension() != _extension)
                return;
                
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

            auto const category_name   = File::without_file_name(std::filesystem::relative(path, _folder_watcher.get_folder_path()));
            auto const category_folder = File::without_file_name(path);
            library.add_definition(*definition, category_name.string(), category_folder.string()); },

        .on_file_removed = [this, &library, &parse_definition](std::filesystem::path const& path) {
            auto identifier = NodeDefinitionIdentifier{
                .definition_name = File::file_name_without_extension(path).string(),
                .category_name = File::without_file_name(std::filesystem::relative(path, _folder_watcher.get_folder_path())).string(),
            };
            library.remove_definition(identifier); },


        .on_file_changed = [](std::filesystem::path const& path) {
            // faire fonction remove_definition
        },

        
        // ToDo fix this error by calling console
        .on_invalid_folder_path = [this](std::filesystem::path const& path) { handle_error(path, "The Nodes folder path is not valid."); },
    };

    _folder_watcher.update(callbacks);

    return true; // TODO(JF) Only return true if a file has changed
}

} // namespace Cool