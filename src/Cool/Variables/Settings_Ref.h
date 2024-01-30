#pragma once
#include <functional>
#include "Settings.h"

namespace Cool {

/// Interface that can wrap several "container of variables"-like objects (aka "settings"-like)
/// and expose what is needed by the presets manager.
class Settings_Ref {
public:
    Settings_Ref(Settings&); // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)

    void assign_from(Settings const&);
    // auto display_all_variables_widgets() -> bool;

private:
    std::reference_wrapper<Settings> _ref;

    friend class Settings_ConstRef;
};

/// Interface that can wrap several "container of variables"-like objects (aka "settings"-like)
/// and expose what is needed by the presets manager.
class Settings_ConstRef {
public:
    Settings_ConstRef(Settings const&); // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
    Settings_ConstRef(Settings_Ref);    // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)

    [[nodiscard]] auto are_equal_to(Settings const&) const -> bool;
    [[nodiscard]] auto as_settings() const -> Settings;

private:
    std::reference_wrapper<Settings const> _ref;
};

} // namespace Cool