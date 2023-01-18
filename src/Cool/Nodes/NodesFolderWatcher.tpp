namespace Cool {

template<NodeDefinition_Concept NodeDefinition, NodeDefinitionParser<NodeDefinition> Parser>
inline auto NodesFolderWatcher::update(NodesLibrary<NodeDefinition>& library, Parser&& parse_definition) -> bool
{
    if (!_has_changed)
        return false;

    _has_changed = false;
    library.clear(); // TODO(JF) Don't do this, only update / remove the files when necessary
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

        auto const category_name = File::without_file_name(std::filesystem::relative(entry.path(), _folder_path));
        library.add_definition(*definition, category_name.string());
    }

    return true; // TODO(JF) Only return true if a file has changed
}

} // namespace Cool