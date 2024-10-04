#pragma once
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Serialization/SerializerOnDemand.h"
#include "wcam/wcam.hpp"

namespace Cool {

auto do_save(wcam::ResolutionsMap const&, Cool::SerializerOnDemand const&) -> bool;
auto do_load(wcam::ResolutionsMap&, Cool::SerializerOnDemand const&) -> Cool::OptionalErrorMessage;

} // namespace Cool