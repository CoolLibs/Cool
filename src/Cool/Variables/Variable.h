#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <stringify/stringify.hpp>
#include "Cool/Log/MessageId.h"
#include "Dirty.h"
#include "VariableTypeC.h"

namespace Cool {

template<VariableTypeC ValueT>
struct Metadata; // You need to create a specialization of this template for each variable type that you want to use.

template<VariableTypeC ValueT>
struct Variable {
    std::string                name{};
    std::optional<std::string> description{};

    ValueT           value{};
    Metadata<ValueT> metadata{};

    ValueT           default_value{value};
    Metadata<ValueT> default_metadata{metadata};

    MessageId message_id{};          // TODO(Variable) Should be stored on the ValueT, only for the types that need it
    DirtyFlag dirty_flag{};          // TODO(Variable) should be a function on_value_changed(), or at least a function that returns which DirtyFlag to set
    int       desired_color_space{}; // TODO(Variable) HACK in order to know which color space to convert to when sending the value to a shader. Only used by Color input.

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
            cereal::make_nvp("Default Metadata", default_metadata),
            cereal::make_nvp("Dirty Flag", dirty_flag),
            cereal::make_nvp("Desired color space", desired_color_space)
            // cereal::make_nvp("Description", description), // (JF): I don't think there is a need to serialize the description since it will be parsed from the shader each time, and applying presets and the like only affect the value of the variable.
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
auto imgui_variable_reset_buttons(Variable<Value>& var, ImGuiVariableCallbacks const& callbacks = {}) -> bool
{
    bool b = false;
    ImGuiExtras::disabled_if(var.value == var.default_value, "Disabled because it is already equal to the default value", [&]() {
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
    if (ImGui::IsItemDeactivatedAfterEdit())
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
