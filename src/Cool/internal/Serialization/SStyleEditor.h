#pragma once
#include <filesystem>
#include "Cool/ImGui/StyleEditor.h"
#include "Cool/Log/OptionalErrorMessage.h"

namespace Cool {

auto do_serialize(StyleEditor const&, std::filesystem::path const&) -> bool;
auto do_deserialize(StyleEditor&, std::filesystem::path const&) -> Cool::OptionalErrorMessage;

} // namespace Cool