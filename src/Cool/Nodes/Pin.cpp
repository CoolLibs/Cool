#include "Pin.h"

namespace Cool {

Pin::Pin(std::string_view name, ax::Drawing::IconType icon)
    : _name{name}
    , _icon{icon}
    , _id{reg::internal::generate_uuid()}
{
}

} // namespace Cool
