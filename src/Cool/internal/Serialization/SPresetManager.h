#pragma once
#include <filesystem>
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Variables/PresetManager.h"
#include "reg/reg.hpp"

namespace Cool {

auto do_save(reg::RawOrderedRegistry<Preset> const&, Cool::SerializerOnDemand const&) -> bool;
auto do_load(reg::RawOrderedRegistry<Preset>&, Cool::SerializerOnDemand const&) -> Cool::OptionalErrorMessage;
void do_load_default_presets(reg::RawOrderedRegistry<Preset>&, std::filesystem::path const&);

} // namespace Cool