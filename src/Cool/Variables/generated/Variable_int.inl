/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_int" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>

namespace Cool {

template<>
struct VariableMetadata<int> {
    internal::BoundsMetadata<int> bounds{
        .min           = 0,
        .max           = 12,
        .has_min_bound = false,
        .has_max_bound = false,
        .drag_speed    = 0.04f,
        .use_slider    = false,
    };

    friend auto operator<=>(const VariableMetadata<int>&, const VariableMetadata<int>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
        archive(
            cereal::make_nvp("Bounds", bounds)
        );
        // #else
        //         (void)archive;
        // #endif
    }
};

auto imgui_widget(Variable<int>&) -> bool;
auto imgui_widget(VariableMetadata<int>&) -> bool;

} // namespace Cool
