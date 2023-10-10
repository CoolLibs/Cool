#pragma once
#include "ed.h"

namespace Cool::internal {

class UniqueEdContext {
public:
    explicit UniqueEdContext()
    {
        ed::Config config;
        config.UserPointer  = this;
        config.LoadSettings = &load_imnodes_settings;
        config.SaveSettings = &save_imnodes_settings;
        _context            = ed::CreateEditor(&config);
    }

    ~UniqueEdContext()
    {
        if (_context)
            ed::DestroyEditor(_context);
    }

    UniqueEdContext(UniqueEdContext const&)                    = delete;
    auto operator=(UniqueEdContext const&) -> UniqueEdContext& = delete;

    UniqueEdContext(UniqueEdContext&& rhs) noexcept                    = delete; // Non-movable because in the ed::Config we pass a pointer to ourselves.
    auto operator=(UniqueEdContext&& rhs) noexcept -> UniqueEdContext& = delete; // This could be worked around though if need be.

    operator ed::EditorContext*() const { return _context; } // NOLINT(*-explicit-constructor, *-explicit-conversions)

private:
    static auto save_imnodes_settings(std::string const& settings, ax::NodeEditor::SaveReasonFlags, void* user_pointer) -> bool
    {
        auto& This             = *reinterpret_cast<UniqueEdContext*>(user_pointer); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
        This._imnodes_settings = settings;
        return true;
    }
    static auto load_imnodes_settings(void* user_pointer) -> std::string
    {
        auto& This = *reinterpret_cast<UniqueEdContext*>(user_pointer); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
        return This._imnodes_settings;
    }

private:
    ed::EditorContext* _context{nullptr};
    std::string        _imnodes_settings{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("UI State", _imnodes_settings)
        );
    }
};

} // namespace Cool::internal