/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_Gradient" function there.
 * -----------------------------------------------------------------------------
 */

            #include <Cool/StrongTypes/Gradient.h>
            #include <Cool/Variables/Variable.h>
            #include <Cool/Variables/internal/BoundsMetadata.h>

            namespace Cool {

            template<>
            struct VariableMetadata<Cool::Gradient> {
                bool is_hdr{false};
bool randomize_new_marks_colors{true};

                friend auto operator<=>(const VariableMetadata<Cool::Gradient>&, const VariableMetadata<Cool::Gradient>&) = default;

            private:
                // Serialisation
                friend class cereal::access;
                template<class Archive>
                void serialize(Archive& archive)
                {
        archive(
cereal::make_nvp("Is HDR", is_hdr),
cereal::make_nvp("Randomize new marks' colors", randomize_new_marks_colors)
        );

                }
            };

            auto imgui_widget(Variable<Cool::Gradient>&) -> bool;
            auto imgui_widget(VariableMetadata<Cool::Gradient>&) -> bool;

            } // namespace Cool
        