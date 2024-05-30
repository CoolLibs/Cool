/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_Time" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/Time/Time.h>
#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>

namespace Cool {

template<>
struct VariableMetadata<Cool::Time> {
    double min{0.};
    double max{10.};
    bool   has_min_bound{false};
    bool   has_max_bound{false};
    float  drag_speed{0.01f};
    bool   show_milliseconds{true};

    friend auto operator<=>(VariableMetadata<Cool::Time> const&, VariableMetadata<Cool::Time> const&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Min", min),
            cereal::make_nvp("Max", max),
            cereal::make_nvp("Has min bound", has_min_bound),
            cereal::make_nvp("Has max bound", has_max_bound),
            cereal::make_nvp("Drag speed", drag_speed),
            cereal::make_nvp("Show milliseconds", show_milliseconds)
        );
    }
};

auto imgui_widget(Variable<Cool::Time>&) -> bool;
auto imgui_widget(VariableMetadata<Cool::Time>&) -> bool;

} // namespace Cool
