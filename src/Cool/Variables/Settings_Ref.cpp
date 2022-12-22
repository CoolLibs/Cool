#include "Settings_Ref.h"
#include "Cool/Variables/Settings_Ref.h"

namespace Cool {

/* ------------------*/
/* Settings_Ref */
/* ------------------*/

Settings_Ref::Settings_Ref(Settings& settings)
    : _ref{settings}
{
}

Settings_Ref::Settings_Ref(SettingsInInputs& settings)
    : _ref{settings}
{
}

/* ------------*/
/* assign_from */
/* ------------*/

static void assign_from_IMPL(std::reference_wrapper<Settings> ref, Settings const& settings)
{
    ref.get() = settings;
}

static void assign_from_IMPL(std::reference_wrapper<SettingsInInputs> ref, Settings const& settings)
{
    // ref.get() = settings;
}

void Settings_Ref::assign_from(Settings const& settings)
{
    std::visit([&](auto&& ref) { assign_from_IMPL(ref, settings); }, _ref);
}

/* ------------------------------*/
/* display_all_variables_widgets */
/* ------------------------------*/

static auto display_all_variables_widgets_IMPL(std::reference_wrapper<Settings> ref) -> bool
{
    bool was_used{false};
    for (auto& variable : ref.get())
    {
        was_used |= std::visit([](auto&& real_variable) { return imgui_widget(real_variable); }, variable);
    }
    return was_used;
}

static auto display_all_variables_widgets_IMPL(std::reference_wrapper<SettingsInInputs> ref) -> bool
{
    return false;
}

auto Settings_Ref::display_all_variables_widgets() -> bool
{
    return std::visit([](auto&& ref) { return display_all_variables_widgets_IMPL(ref); }, _ref);
}

/* ------------------*/
/* Settings_ConstRef */
/* ------------------*/

Settings_ConstRef::Settings_ConstRef(Settings const& settings)
    : _ref{settings}
{
}

Settings_ConstRef::Settings_ConstRef(SettingsInInputs const& settings)
    : _ref{settings}
{
}

Settings_ConstRef::Settings_ConstRef(Settings_Ref ref)
    : _ref{std::visit([](auto&& ref) { return Ref{ref}; }, ref._ref)}
{
}

/* -------------*/
/* are_equal_to */
/* -------------*/

static auto are_equal_to_IMPL(std::reference_wrapper<Settings const> ref, Settings const& settings) -> bool
{
    return ref.get() == settings;
}

static auto are_equal_to_IMPL(std::reference_wrapper<SettingsInInputs const> ref, Settings const& settings) -> bool
{
    return false;
    // return ref.get() == settings;
}

auto Settings_ConstRef::are_equal_to(Settings const& settings) const -> bool
{
    return std::visit([&](auto&& ref) { return are_equal_to_IMPL(ref, settings); }, _ref);
}

/* ------------*/
/* as_settings */
/* ------------*/

static auto as_settings_IMPL(std::reference_wrapper<Settings const> ref) -> Settings
{
    return ref.get();
}

static auto as_settings_IMPL(std::reference_wrapper<SettingsInInputs const> ref) -> Settings
{
    return {};
    // return ref.get() == settings;
}

auto Settings_ConstRef::as_settings() const -> Settings
{
    return std::visit([](auto&& ref) { return as_settings_IMPL(ref); }, _ref);
}

} // namespace Cool