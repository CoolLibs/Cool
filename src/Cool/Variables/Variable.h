#pragma once
#include <Cool/ImGui/ImGuiExtras.h>
#include <gsl/pointers>
#include <memory>
#include <stringify/stringify.hpp>
#include "Cool/Log/MessageId.h"

namespace Cool {

// TODO(Variable) Make many things in Coollab variables, so that users can customize GUI and reset to default values. This shouldn't only be for nodes params.

template<typename T>
struct VariableMetadata; // You need to create a specialization of this template for each variable type that you want to use.

template<typename T>
struct VariableData {
    std::string         name{};
    T                   value{};
    VariableMetadata<T> metadata{};

    friend auto operator==(VariableData const&, VariableData const&) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", name),
            cereal::make_nvp("Value", value),
            cereal::make_nvp("Metadata", metadata)
        );
    }
};

/// A simple object that wraps a value with some metadata,
/// allowing the user to customize the UI,
/// and reset to the default value.
template<typename T>
class Variable {
public:
    explicit Variable(VariableData<T> const& data = {})
        : _name{data.name}
        , _value{data.value}
        , _metadata{data.metadata}
        , _default_value{data.value}
        , _default_metadata{data.metadata}
    {}

    // friend auto operator==(Variable const& a, Variable const& b) -> bool
    // {
    //     return a.data == b.data;
    // }

    // auto name() -> auto& { return _name; }
    auto value() -> auto& { return _value; }
    auto metadata() -> auto& { return _metadata; }
    // auto default_value() -> auto& { return _default_value; }       // TODO(Variables) Remove ?
    // auto default_metadata() -> auto& { return _default_metadata; } // TODO(Variables) Remove ?

    auto name() const -> auto const& { return _name; }
    auto value() const -> auto const& { return _value; }
    auto metadata() const -> auto const& { return _metadata; }
    auto default_value() const -> auto const& { return _default_value; }       // TODO(Variables) Remove ?
    auto default_metadata() const -> auto const& { return _default_metadata; } // TODO(Variables) Remove ?

private:
    std::string                      _name{};
    T                                _value{};
    VariableMetadata<T>              _metadata{};
    T /*  const */                   _default_value{};    // NOLINT(*avoid-const-or-ref-data-members)
    VariableMetadata<T> /*  const */ _default_metadata{}; // NOLINT(*avoid-const-or-ref-data-members)

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", _name),
            cereal::make_nvp("Value", _value),
            cereal::make_nvp("Metadata", _metadata),
            cereal::make_nvp("Default Value", _default_value),
            cereal::make_nvp("Default Metadata", _default_metadata)
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
    ImGuiExtras::disabled_if(var.value() == var.default_value(), "Disabled because it is already equal to the default value", [&]() {
        if (ImGui::Button(("Reset to default value (" + stringify(var.default_value()) + ")").c_str()))
        {
            var.value() = var.default_value();
            b           = true;
            callbacks.on_value_changed();
            callbacks.on_value_editing_finished();
        }
    });
    if (var.metadata() != var.default_metadata())
    {
        ImGui::SameLine();
        if (ImGui::Button("Reset Metadata"))
        {
            var.metadata() = var.default_metadata();
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

    if (ImGui::BeginPopupContextItem(var.name().c_str()))
    {
        ImGui::PushID(&var + 1);
        ImGui::BeginGroup();
        {
            if (imgui_widget(var.metadata()))
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
