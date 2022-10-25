#include <Cool/Dependencies/Dirty.h>
#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/Dependencies/InputProvider_Ref.h>
#include <tl/expected.hpp>

namespace Cool {

/// Parses all the INPUT lines from `text`.
/// See try_parse_input() to know what an INPUT line is.
/// Might return an error message instead of the inputs if the parsing failed.
auto parse_all_inputs(
    std::string_view text,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> tl::expected<std::vector<AnyInput>, std::string>;

/// Takes a line of the form "INPUT float x; // 0.5 min 0 max 1"
/// and returns an Input with the corresponding metadata (or std::nullopt if the parsing failed).
/// Some metadatas can be missing from the string and will be constructed with their default value.
auto try_parse_input(
    std::string_view line,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::optional<AnyInput>;

auto preprocess_inputs(std::string_view source_code, const std::vector<AnyInput>&, Cool::InputProvider_Ref) -> std::string;

} // namespace Cool