#include "OSCConnectionEndpoint.h"

namespace Cool {

auto OSCConnectionEndpoint::is_valid() const -> bool
{
    return port >= 0;
}

} // namespace Cool