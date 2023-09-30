#pragma once
#include <filesystem>
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Variables/PresetManager.h"
#include "reg/reg.hpp"

namespace Cool {

auto do_save(reg::RawOrderedRegistry<Preset2> const&, Cool::SerializerOnDemand const&) -> bool;
auto do_load(reg::RawOrderedRegistry<Preset2>&, Cool::SerializerOnDemand const&) -> Cool::OptionalErrorMessage;
void do_load_default_presets(reg::RawOrderedRegistry<Preset2>&, std::filesystem::path const&);

} // namespace Cool