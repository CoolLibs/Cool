#pragma once
#include <Cool/Dependencies/Input.h>
#include <functional>
#include ""
#include "Settings.h"


namespace Cool {

struct SettingsInInputs {
    std::reference_wrapper<std::vector<Cool::AnyInput>> inputs;
    CommandExecutor_TopLevel_Ref                        command_executor;
};

struct SettingsInInputsConst {
    std::reference_wrapper<std::vector<Cool::AnyInput> const> inputs;
    std::reference_wrapper<Cool::VariableRegistries const>    registry;
};

/// Interface that can wrap several "container of variables"-like objects (aka "settings"-like)
/// and expose what is needed by the presets manager.
class Settings_Ref {
public:
    Settings_Ref(Settings&);         // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
    Settings_Ref(SettingsInInputs);  // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)

    void assign_from(Settings const&);
    auto display_all_variables_widgets() -> bool;

private:
    using Ref = std::variant<
        std::reference_wrapper<Settings>,
        SettingsInInputs>;
    Ref _ref;

    friend class Settings_ConstRef;
};

/// Interface that can wrap several "container of variables"-like objects (aka "settings"-like)
/// and expose what is needed by the presets manager.
class Settings_ConstRef {
public:
    Settings_ConstRef(Settings const&);         // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
    Settings_ConstRef(SettingsInInputsConst);   // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
    Settings_ConstRef(Settings_Ref);            // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)

    [[nodiscard]] auto are_equal_to(Settings const&) const -> bool;
    [[nodiscard]] auto as_settings() const -> Settings;

private:
    using Ref = std::variant<
        std::reference_wrapper<Settings const>,
        SettingsInInputsConst>;
    Ref _ref;
};

} // namespace Cool