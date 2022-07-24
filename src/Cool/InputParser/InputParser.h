#include <Cool/Dependencies/Dirty.h>
#include <Cool/Dependencies/InputFactory_Ref.h>

namespace Cool {

// Call try_parse_input and add the returning input to the list
auto parse_all_inputs(
    std::string_view text,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::vector<AnyInput>;

// Try to parse input by finding type, name and verify if the line is commented out
auto try_parse_input(
    std::string_view line,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::optional<AnyInput>;

} // namespace Cool