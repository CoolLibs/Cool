#pragma once

#include <reg/reg.hpp>
#include "Pin.h"

namespace Cool {

struct Link {
    PinId from_pin_id;
    PinId to_pin_id;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
#if COOL_SERIALIZATION
        archive(
            cereal::make_nvp("From pin", from_pin_id),
            cereal::make_nvp("To pin", to_pin_id)
        );
#else
        (void)archive;
#endif
    }
};

using LinkId = reg::Id<Link>;

} // namespace Cool