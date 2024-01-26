#include "UniqueEdContext.h"

namespace Cool::internal {

UniqueEdContext::UniqueEdContext()
{
    ed::Config config;
    config.UserPointer    = this;
    config.LoadSettings   = &load_imnodes_settings;
    config.SaveSettings   = &save_imnodes_settings;
    _context              = ed::CreateEditor(&config);
}

UniqueEdContext::~UniqueEdContext()
{
    if (_context)
        ed::DestroyEditor(_context);
}

auto UniqueEdContext::save_imnodes_settings(std::string const& settings, ax::NodeEditor::SaveReasonFlags, void* user_pointer) -> bool
{
    auto& This             = *reinterpret_cast<UniqueEdContext*>(user_pointer); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    This._imnodes_settings = settings;
    return true;
}

auto UniqueEdContext::load_imnodes_settings(void* user_pointer) -> std::string
{
    auto& This = *reinterpret_cast<UniqueEdContext*>(user_pointer); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    return This._imnodes_settings;
}

} // namespace Cool::internal