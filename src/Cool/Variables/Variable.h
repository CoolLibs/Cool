#pragma once

namespace Cool {

template<typename Value>
struct VariableMetadata {
    VariableMetadata() = delete; // You should create a specialization of this template for each variable type that you want to use.
};

template<typename Value>
struct Variable {
    std::string             name{};
    Value                   value{};
    VariableMetadata<Value> metadata{};
    Value                   default_value{value};
    VariableMetadata<Value> default_metadata{metadata};

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Name", name),
                cereal::make_nvp("Value", value),
                cereal::make_nvp("Metadata", metadata),
                cereal::make_nvp("Default Value", default_value),
                cereal::make_nvp("Default Metadata", default_metadata));
    }
};

/// Returns true iff the `value` of the variable changed
/// (Currently there is no way to know if the metadata changed)
template<typename Value>
auto imgui_variable_reset_buttons(Variable<Value>& var) -> bool
{
    bool b = false;
    if (ImGui::Button("Reset Value")) {
        var.value = var.default_value;
        b         = true;
    }
    if (var.metadata != var.default_metadata) {
        ImGui::SameLine();
        if (ImGui::Button("Reset Metadata")) {
            var.metadata = var.default_metadata;
        }
    }
    return b;
}

/// Returns true iff the `value` of the variable changed
/// (Currently there is no way to know if the metadata changed)
template<typename Value>
auto imgui(Variable<Value>& var) -> bool
{
    bool b = false;
    ImGui::PushID(&var);
    {
        b |= imgui_widget(var);
    }
    ImGui::PopID();
    ImGui::PushID(&var + 17);
    {
        if (ImGui::BeginPopupContextItem(var.name.c_str())) {
            imgui_widget(var.metadata);
            b |= imgui_variable_reset_buttons(var);
            ImGui::EndPopup();
        }
    }
    ImGui::PopID();
    return b;
}

} // namespace Cool
