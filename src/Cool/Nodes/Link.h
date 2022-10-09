#pragma once

#include <reg/reg.hpp>
#include "Pin.h"

namespace Cool::Nodes {

struct Link {
    PinId from_pin_id;
    PinId to_pin_id;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("From pin", from_pin_id),
            cereal::make_nvp("To pin", to_pin_id)
        );
    }
};

using LinkId = reg::Id<Link>;

} // namespace Cool::Nodes