/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_MathExpression" function there.
 * -----------------------------------------------------------------------------
 */

            #include <Cool/StrongTypes/MathExpression.h>
            #include <Cool/Variables/Variable.h>
            #include <Cool/Variables/internal/BoundsMetadata.h>

            namespace Cool {

            template<>
            struct VariableMetadata<Cool::MathExpression> {
                

                friend auto operator<=>(const VariableMetadata<Cool::MathExpression>&, const VariableMetadata<Cool::MathExpression>&) = default;

            private:
                // Serialisation
                friend class cereal::access;
                template<class Archive>
                void serialize(Archive&)
                {
                }
            };

            auto imgui_widget(Variable<Cool::MathExpression>&) -> bool;
            auto imgui_widget(VariableMetadata<Cool::MathExpression>&) -> bool;

            } // namespace Cool
        