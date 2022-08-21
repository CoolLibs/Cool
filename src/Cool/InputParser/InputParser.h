#include <Cool/Dependencies/Dirty.h>
#include <Cool/Dependencies/InputFactory_Ref.h>

namespace Cool {

/// Parses all the INPUT lines from `text`.
/// See try_parse_input() to know what an INPUT line is.
auto parse_all_inputs(
    std::string_view text,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::vector<AnyInput>;

/// Takes a line of the form "INPUT float x; // 0.5 min 0 max 1"
/// and returns an Input with the corresponding metadata (or std::nullopt if the parsing failed).
/// Some metadatas can be missing from the string and will be constructed with their default value.
auto try_parse_input(
    std::string_view line,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::optional<AnyInput>;

auto preprocess_inputs(std::string_view source_code) -> std::string;

} // namespace Cool