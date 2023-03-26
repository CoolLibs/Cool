/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_vec2" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>
#include <glm/glm.hpp>

namespace Cool {

template<>
struct VariableMetadata<glm::vec2> {
    internal::BoundsMetadata<float> bounds{
        .min           = 0.f,
        .max           = 1.f,
        .has_min_bound = false,
        .has_max_bound = false,
        .drag_speed    = 0.01f,
        .use_slider    = false,
    };

    friend auto operator<=>(const VariableMetadata<glm::vec2>&, const VariableMetadata<glm::vec2>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
#if COOL_SERIALIZATION
        archive(
            cereal::make_nvp("Bounds", bounds)
        );
#else
        (void)archive;
#endif
    }
};

auto imgui_widget(Variable<glm::vec2>&) -> bool;
auto imgui_widget(VariableMetadata<glm::vec2>&) -> bool;

} // namespace Cool
