/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_debug_options.py" and edit the "DebugOptionsCpp" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/Path/Path.h>
#include <Cool/Serialization/Serialization.h>
#include <ser20/archives/json.hpp>

namespace Cool {

void DebugOptions::save_to_file()
{
    Cool::Serialization::save<DebugOptions::Instance, ser20::JSONOutputArchive>(
        instance(),
        Cool::Path::user_data() / "debug-options-cool.json",
        "Debug Options"
    );
}

auto DebugOptions::load_debug_options() -> Instance
{
    auto       the_instance = Instance{};
    auto const err          = Cool::Serialization::load<DebugOptions::Instance, ser20::JSONInputArchive>(the_instance, Cool::Path::user_data() / "debug-options-cool.json");
    std::ignore             = err; // We don't care about preserving the backward compatibility of debug options. If we break it, we can ignore it.
    return the_instance;
}

} // namespace Cool
