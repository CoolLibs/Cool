#include "Exporter.h"
#include "ExporterU.h"

namespace Cool {

Exporter::Exporter(Exporter&& o) noexcept
    : _gui{std::move(o._gui)}
// We don't move the export process because it is immovable + we typically don't want to move it, we just want to copy the params in the gui
{
}

auto Exporter::operator=(Exporter&& o) noexcept -> Exporter&
{
    _gui = std::move(o._gui);
    // We don't move the export process because it is immovable + we typically don't want to move it, we just want to copy the params in the gui
    return *this;
}

auto Exporter::clock() const -> Clock const&
{
    assert(_video_export_process.has_value() && "Can only be called if `is_exporting()` is true");
    return _video_export_process->clock(); // NOLINT(bugprone-unchecked-optional-access)
}

auto Exporter::export_image_with_current_settings_using_a_task(Time time, Time delta_time, Polaroid const& polaroid, std::function<bool(std::filesystem::path const&)> const& extra_conditions_to_consider_image_path_valid) -> std::filesystem::path
{
    auto path = File::find_available_path(_gui.image_export_path(), extra_conditions_to_consider_image_path_valid);
    ExporterU::export_image_using_a_task(_gui.export_size(), time, delta_time, polaroid, path);
    return path;
}

} // namespace Cool