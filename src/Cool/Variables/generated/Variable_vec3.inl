/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_vec3" function there.
 * -----------------------------------------------------------------------------
 */

            #include <glm/glm.hpp>
            #include <Cool/Variables/Variable.h>
            #include <Cool/Variables/internal/BoundsMetadata.h>

            namespace Cool {

            template<>
            struct VariableMetadata<glm::vec3> {
                internal::BoundsMetadata<float> bounds{
                .min = 0.f,
                .max = 1.f,
                .has_min_bound = false,
                .has_max_bound = false,
                .drag_speed = 0.01f,
                .use_slider = false,
            };

                friend auto operator<=>(const VariableMetadata<glm::vec3>&, const VariableMetadata<glm::vec3>&) = default;

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

            auto imgui_widget(Variable<glm::vec3>&) -> bool;
            auto imgui_widget(VariableMetadata<glm::vec3>&) -> bool;

            } // namespace Cool
        