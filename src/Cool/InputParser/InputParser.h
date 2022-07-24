#include <Cool/Dependencies/Dirty.h>
#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/StrongTypes/RgbColor.h>
#include <Cool/Variables/Variable.h>

namespace Cool {

// TODO(LD) DOCUMENTATION
auto parse_all_inputs(
    std::string_view text,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::vector<AnyInput>;

// TODO(LD) DOCUMENTATION
auto try_parse_input(
    std::string_view line,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::optional<AnyInput>;

} // namespace Cool