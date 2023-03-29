#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <stringify/stringify.hpp>
#include "Cool/Log/MessageId.h"

namespace Cool {

template<typename Value>
struct VariableMetadata; // You need to create a specialization of this template for each variable type that you want to use.

template<typename Value>
struct Variable {
    struct Data {
        std::string             name{};
        Value                   value{};
        VariableMetadata<Value> metadata{};
        Value                   default_value{value};
        VariableMetadata<Value> default_metadata{metadata};

        friend auto operator==(Data const&, Data const&) -> bool = default;
    };
    ~Variable() = default;

    Variable(Data data = {}) // NOLINT(*-explicit-constructor, *-explicit-conversions)
        : data{std::move(data)}
    {}

    auto name() -> auto& { return data.name; }
    auto value() -> auto& { return data.value; }
    auto metadata() -> auto& { return data.metadata; }
    auto default_value() -> auto& { return data.default_value; }
    auto default_metadata() -> auto& { return data.default_metadata; }

    auto name() const -> auto const& { return data.name; }
    auto value() const -> auto const& { return data.value; }
    auto metadata() const -> auto const& { return data.metadata; }
    auto default_value() const -> auto const& { return data.default_value; }
    auto default_metadata() const -> auto const& { return data.default_metadata; }

    Data data{};

    MessageId message_id{};

    friend auto operator==(Variable const& a, Variable const& b) -> bool
    {
        return a.data == b.data;
    }

    Variable(Variable const& other)
        : data{other.data}
    {}

    Variable(Variable&& other) noexcept
        : data{std::move(other.data)}
        , message_id{std::move(other.message_id)}
    {}

    auto operator=(Variable const& other) -> Variable&
    {
        if (this == &other)
            return *this;
        data = other.data;
        return *this;
    }

    auto operator=(Variable&& other) noexcept -> Variable&
    {
        if (this == &other)
            return *this;
        data       = std::move(other.data);
        message_id = std::move(other.message_id);
        return *this;
    }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", data.name),
            cereal::make_nvp("Value", data.value),
            cereal::make_nvp("Metadata", data.metadata),
            cereal::make_nvp("Default Value", data.default_value),
            cereal::make_nvp("Default Metadata", data.default_metadata)
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
    ImGuiExtras::maybe_disabled(var.value() == var.default_value(), "Disabled because it is already equal to the default value", [&]() {
        if (ImGui::Button(("Reset to default value (" + stringify(var.default_value()) + ")").c_str()))
        {
            var.value() = var.default_value();
            b         = true;
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
