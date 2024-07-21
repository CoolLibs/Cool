#pragma once
#include <reg/reg.hpp>
#include "Pin.h"

namespace Cool {

struct Link {
    PinId from_pin_id;
    PinId to_pin_id;

    friend auto operator==(Link const&, Link const&) -> bool = default;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("From pin", from_pin_id),
            ser20::make_nvp("To pin", to_pin_id)
        );
    }
};

using LinkId = reg::Id<Link>;

} // namespace Cool