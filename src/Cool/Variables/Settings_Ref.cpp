#include "Settings_Ref.h"

namespace Cool {

Settings_Ref::Settings_Ref(Settings&)
{
}

void Settings_Ref::assign_from(Settings const&)
{
}

auto Settings_Ref::display_all_variables_widgets() -> bool
{
    //     bool was_used{false};
    //     for (auto& variable : settings)
    //     {
    //         was_used |= std::visit([](auto&& real_variable) { return imgui_widget(real_variable); }, variable);
    //     }
    //     return was_used;
    return {};
}

Settings_ConstRef::Settings_ConstRef(Settings const&)
{
}

Settings_ConstRef::Settings_ConstRef(Settings_Ref)
{
}

auto Settings_ConstRef::are_equal_to(Settings const&) const -> bool
{
    return {};
}

auto Settings_ConstRef::as_settings() const -> Settings
{
    return {};
}

} // namespace Cool