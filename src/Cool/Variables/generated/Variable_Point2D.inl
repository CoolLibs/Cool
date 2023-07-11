/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_Point2D" function there.
 * -----------------------------------------------------------------------------
 */

            #include <Cool/StrongTypes/Point2D.h>
            #include <Cool/Variables/Variable.h>
            #include <Cool/Variables/internal/BoundsMetadata.h>

            namespace Cool {

            template<>
            struct VariableMetadata<Cool::Point2D> {
                float drag_speed{0.0001f};

                friend auto operator<=>(const VariableMetadata<Cool::Point2D>&, const VariableMetadata<Cool::Point2D>&) = default;

            private:
                // Serialisation
                friend class cereal::access;
                template<class Archive>
                void serialize(Archive& archive)
                {
        archive(
cereal::make_nvp("Drag speed", drag_speed)
        );

                }
            };

            auto imgui_widget(Variable<Cool::Point2D>&) -> bool;
            auto imgui_widget(VariableMetadata<Cool::Point2D>&) -> bool;

            } // namespace Cool
        