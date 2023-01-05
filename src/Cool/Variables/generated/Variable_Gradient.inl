/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_Gradient" function there.
 * -----------------------------------------------------------------------------
 */

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