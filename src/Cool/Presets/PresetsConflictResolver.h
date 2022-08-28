#pragma once

#include "Settings.h"

namespace Cool {

using

    class PresetsConflictResolver {
public:
    auto has_conflicts() -> bool { return true; }

    /// Returns true iff all conflicts are resolved.
    auto imgui() -> bool;

private:
    Settings _new_signature;
    Settings _old_signature;
};

} // namespace Cool