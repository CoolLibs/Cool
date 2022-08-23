#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <stringify/stringify.hpp>

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

    friend bool operator==(const Variable<Value>&, const Variable<Value>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", name),
            cereal::make_nvp("Value", value),
            cereal::make_nvp("Metadata", metadata),
            cereal::make_nvp("Default Value", default_value),
            cereal::make_nvp("Default Metadata", default_metadata)
        );
    }
};

struct ImGuiVariableCallbacks {
    std::function<void()> on_value_changed = []() {
    };
    std::function<void()> on_metadata_changed = []() {
    };
    std::function<void()> on_value_editing_finished = []() {
    };
    std::function<void()> on_metadata_editing_finished = []() {
    };
};

/// Returns true iff the `value` of the variable changed
/// (Currently there is no way to know if the metadata changed)
template<typename Value>
auto imgui_variable_reset_buttons(Variable<Value>& var, ImGuiVariableCallbacks callbacks = {}) -> bool
{
    bool b = false;
    ImGuiExtras::maybe_disabled(var.value == var.default_value, "Disabled because it is already equal to the default value", [&]() {
        if (ImGui::Button(("Reset to default value (" + stringify(var.default_value) + ")").c_str()))
        {
            var.value = var.default_value;
            b         = true;
            callbacks.on_value_changed();
            callbacks.on_value_editing_finished();
        }
    });
    if (var.metadata != var.default_metadata)
    {
        ImGui::SameLine();
        if (ImGui::Button("Reset Metadata"))
        {
            var.metadata = var.default_metadata;
            callbacks.on_metadata_changed();
            callbacks.on_metadata_editing_finished();
        }
    }
    return b;
}

/// Returns true iff the `value` of the variable changed
/// Calls the corresponding callback if either `value` or `metadata` change
template<typename Value>
auto imgui(
    Variable<Value>&       var,
    ImGuiVariableCallbacks callbacks = {}
) -> bool
{
    bool b = false;

    ImGui::PushID(&var);
    ImGui::BeginGroup();
    {
        if (imgui_widget(var))
        {
            b = true;
            callbacks.on_value_changed();
        }
    }
    ImGui::EndGroup();
    ImGui::PopID();
    if (ImGui::IsItemDeactivatedAfterEdit()) // TODO(ASG) Make sure this boolean is true when Gradient adds a mark, removes one etc.
    {
        callbacks.on_value_editing_finished();
    }

    if (ImGui::BeginPopupContextItem(var.name.c_str()))
    {
        ImGui::PushID(&var + 1);
        ImGui::BeginGroup();
        {
            if (imgui_widget(var.metadata))
            {
                callbacks.on_metadata_changed();
            }
        }
        ImGui::EndGroup();
        ImGui::PopID();
        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            callbacks.on_metadata_editing_finished();
        }
        b |= imgui_variable_reset_buttons(var, callbacks);
        ImGui::EndPopup();
    }

    return b;
}

} // namespace Cool
