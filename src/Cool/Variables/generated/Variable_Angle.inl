/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_variables.py" and edit the "Variable_Angle" function there.
 * -----------------------------------------------------------------------------
 */

template<>
struct VariableMetadata<Cool::Angle> {
    friend auto operator<=>(const VariableMetadata<Cool::Angle>&, const VariableMetadata<Cool::Angle>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};