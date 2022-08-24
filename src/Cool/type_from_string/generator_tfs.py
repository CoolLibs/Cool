# ---HOW TO---
# Modify `all_associations()` and `includes()`, then run this script.
# ------------


tfs_global_type_to_string_associations = {}
tfs_global_includes = ""


def includes():
    return tfs_global_includes


def string_to_type_associations():
    out = "\n"
    for key, values in tfs_global_type_to_string_associations.items():
        for value in values:
            out += f"""
template<>
struct from_impl<"{value}"> {{
    using type = {key};
}};
"""
    return out


def evaluate_function_template():
    out = "#define COOL_TFS_EVALUATE_FUNCTION_TEMPLATE(function_template, type_as_string, out_type, arguments) \\\n"
    out += "([&]() -> out_type { \\\n"
    is_first = True
    for key, values in tfs_global_type_to_string_associations.items():
        for value in values:
            out += f'{"else " if not is_first else ""}if ((type_as_string) == "{value}") return function_template<{key}>arguments;' + "\\\n"
            is_first = False

    out += f'else {{ throw std::runtime_error{{"Unknown type \\"" + std::string{{type_as_string}} + "\\""}};  }} \\\n}})()'
    out += f'\n'
    return out


def main(all_type_to_string_associations, all_includes):
    # HACK: Python doesn't allow us to import from a parent folder (e.g. tooling.generate_files)
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path
    sys.path.append(os.path.join(
        Path(os.path.abspath(__file__)).parent.parent.parent.parent,
        "tooling")
    )
    # End of HACK
    import generate_files

    global tfs_global_type_to_string_associations, tfs_global_includes
    tfs_global_type_to_string_associations = all_type_to_string_associations
    tfs_global_includes = all_includes

    generate_files.generate(
        folder=os.path.join(
            Path(os.path.abspath(__file__)).parent, "generated"),
        files=[
            string_to_type_associations,
            includes,
            evaluate_function_template,
        ],
        calling_file=__file__,
    )


if __name__ == '__main__':
    # HACK: Python doesn't allow us to import from a parent folder
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path
    sys.path.append(os.path.join(
        Path(os.path.abspath(__file__)).parent.parent,
        "Variables")
    )
    # End of HACK
    import generator_variables
    main(
        generator_variables.all_types_representations_as_strings(),
        generator_variables.all_types_includes(),
    )
