#include "Pin.h"

namespace Cool {

Pin::Pin(std::string_view name)
    : _name{name}
    , _id{reg::internal::generate_uuid()}
{
}

} // namespace Cool
