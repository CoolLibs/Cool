/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_variables.py" and edit the "Variable_vec3" function there.
 * -----------------------------------------------------------------------------
 */


template<>
struct VariableMetadata<glm::vec3> {
    

    friend auto operator<=>(const VariableMetadata<glm::vec3>&, const VariableMetadata<glm::vec3>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};