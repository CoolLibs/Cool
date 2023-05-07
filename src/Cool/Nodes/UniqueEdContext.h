#pragma once
#include "ed.h"

namespace Cool::internal {

class UniqueEdContext {
public:
    explicit UniqueEdContext(std::filesystem::path const& serialization_file)
    {
        ed::Config config;
        config.SettingsFile = serialization_file.string();
        _context            = ed::CreateEditor(&config);
    }

    ~UniqueEdContext()
    {
        if (_context)
            ed::DestroyEditor(_context);
    }

    UniqueEdContext(UniqueEdContext const&)                    = delete;
    auto operator=(UniqueEdContext const&) -> UniqueEdContext& = delete;

    UniqueEdContext(UniqueEdContext&& rhs) noexcept
        : _context{rhs._context}
    {
        rhs._context = nullptr;
    };

    auto operator=(UniqueEdContext&& rhs) noexcept -> UniqueEdContext&
    {
        if (this != &rhs)
        {
            ed::DestroyEditor(_context);
            _context     = rhs._context;
            rhs._context = nullptr;
        }
        return *this;
    }

    operator ed::EditorContext*() const { return _context; } // NOLINT(*-explicit-constructor, *-explicit-conversions)

private:
    ed::EditorContext* _context{nullptr};
};

} // namespace Cool::internal