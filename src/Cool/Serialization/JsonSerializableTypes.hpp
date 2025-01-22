#pragma once
#include "Cool/Exporter/VideoExportOverwriteBehaviour.h"
#include "Cool/Time/Time.hpp"

namespace Cool {

using JsonSerializableTypes = std::variant<bool, float, Time, VideoExportOverwriteBehaviour>;

}