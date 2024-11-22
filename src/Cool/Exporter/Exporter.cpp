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

auto Exporter::export_image_immediately(Time time, Time delta_time, Polaroid const& polaroid, std::function<void(std::filesystem::path const& exported_image_path)> const& on_image_exported) -> bool
{
    return ExporterU::export_image(_gui.export_size(), time, delta_time, polaroid, _gui.image_export_path(), on_image_exported);
}

} // namespace Cool