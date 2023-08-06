/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_debug_options.py" and edit the "DebugOptionsCpp" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/Path/Path.h>
#include <Cool/Serialization/Serialization.h>
#include <cereal/archives/json.hpp>

namespace Cool {

void DebugOptions::save_to_file()
{
    Cool::Serialization::save<DebugOptions::Instance, cereal::JSONOutputArchive>(
        instance(),
        Cool::Path::user_data() / "debug-options-cool.json",
        "Debug Options"
    );
}

auto DebugOptions::load_debug_options() -> Instance
{
    auto the_instance = Instance{};
    Cool::Serialization::load<DebugOptions::Instance, cereal::JSONInputArchive>(the_instance, Cool::Path::user_data() / "debug-options-cool.json")
        .send_error_if_any([](const std::string& message) {
            return Cool::Message{
                .category = "Loading Debug Options",
                .message  = message,
                .severity = Cool::MessageSeverity::Warning,
            };
        },
                           Cool::Log::ToUser::console());
    return the_instance;
}

} // namespace Cool
