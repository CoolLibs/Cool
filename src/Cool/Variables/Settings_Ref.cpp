#include "Settings_Ref.h"

namespace Cool {

Settings_Ref::Settings_Ref(Settings& settings)
    : _ref{settings}
{
}

void Settings_Ref::assign_from(Settings const& settings)
{
    _ref.get() = settings;
}

Settings_ConstRef::Settings_ConstRef(Settings const& settings)
    : _ref{settings}
{
}

Settings_ConstRef::Settings_ConstRef(Settings_Ref ref)
    : _ref{ref._ref}
{
}

auto Settings_ConstRef::are_equal_to(Settings const& settings) const -> bool
{
    return _ref.get() == settings;
}

auto Settings_ConstRef::as_settings() const -> Settings
{
    return _ref.get();
}

} // namespace Cool