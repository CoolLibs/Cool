#include "Pin.h"
#include "as_ed_id.h"
#include "ed.h"

namespace Cool {

Pin::Pin(std::string_view name)
    : _name{name}
    , _id{reg::internal::generate_uuid()}
{
}

} // namespace Cool
