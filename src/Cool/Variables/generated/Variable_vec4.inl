/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_vec4" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>
#include <glm/glm.hpp>

namespace Cool {

template<>
struct VariableMetadata<glm::vec4> {
    internal::BoundsMetadata<float> bounds{
        .min        = 0.f,
        .max        = 1.f,
        .drag_speed = 0.01f,
        .is_bounded = false,
    };

    friend auto operator<=>(const VariableMetadata<glm::vec4>&, const VariableMetadata<glm::vec4>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Bounds", bounds)
        );
    }
};

auto imgui_widget(Variable<glm::vec4>&) -> bool;
auto imgui_widget(VariableMetadata<glm::vec4>&) -> bool;

} // namespace Cool
