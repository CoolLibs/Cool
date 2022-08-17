/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_float" function there.
 * -----------------------------------------------------------------------------
 */

template<>
struct VariableMetadata<float> {
    float min_value{0.f};
    float max_value{1.f};

    friend auto operator<=>(const VariableMetadata<float>&, const VariableMetadata<float>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Min Value", min_value),
            cereal::make_nvp("Max Value", max_value)
        );
    }
};