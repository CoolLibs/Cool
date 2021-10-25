#include "Uuid.h"
#include <Cool/Random/Random.h>

namespace Cool {

Uuid::Uuid()
    : _uuid{Random::uint64()}
{
}

} // namespace Cool
