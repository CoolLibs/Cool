#pragma once
#include <ranges>
#include "Pin.h"

namespace Cool::Nodes {

template<typename T>
concept NodesCfg_Concept = requires(T nodes_cfg)
{
    typename T::NodeT;
};

} // namespace Cool::Nodes