/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_variables.py" and edit the "Variable_Direction2D" function there.
 * -----------------------------------------------------------------------------
 */

template<>
struct VariableMetadata<Cool::Direction2D> {
    friend auto operator<=>(const VariableMetadata<Cool::Direction2D>&, const VariableMetadata<Cool::Direction2D>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};