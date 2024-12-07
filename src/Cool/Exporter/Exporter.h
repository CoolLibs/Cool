#pragma once
#include "Cool/Exporter/Polaroid.hpp"
#include "ExporterGui.h"
#include "VideoExportProcess.h"

namespace Cool {

class Exporter {
public:
    Exporter()  = default;
    ~Exporter() = default;
    Exporter(Exporter&&) noexcept;
    auto operator=(Exporter&&) noexcept -> Exporter&;

    Exporter(const Exporter&)                    = delete; // I don't expect anyone will need this
    auto operator=(const Exporter&) -> Exporter& = delete; // But if so, we could implement it.

    [[nodiscard]] auto is_exporting() const -> bool { return Cool::ExporterGui::is_exporting(_video_export_process); }

    auto image_export_window() -> ImGuiWindow& { return _gui.image_export_window(); }
    auto video_export_window() -> ImGuiWindow& { return _gui.video_export_window(); }

    /// Displays all the currently active windows.
    void imgui_windows(exporter_imgui_windows_Params const& p) { _gui.imgui_windows(p, _video_export_process); }

    /// The buttons to open the different exporter windows.
    void imgui_menu_items(exporter_imgui_menu_items_Params const& params = {}, std::optional<std::string> const& longest_text = {}) { _gui.imgui_menu_items(params, longest_text); }

    /// Call this after your rendering code. If we are exporting it will export the current frame and decide if the export should continue.
    void update(Polaroid const& polaroid) { _gui.update(polaroid, _video_export_process); }

    /// Can only be called if `is_exporting()` is true.
    auto clock() const -> Clock const&;

    void set_shared_aspect_ratio(SharedAspectRatio& shared_aspect_ratio) { _gui.set_shared_aspect_ratio(shared_aspect_ratio); }

    auto export_image_immediately(Time time, Time delta_time, Polaroid const& polaroid, std::function<void(std::filesystem::path const& exported_image_path)> const& on_image_exported) -> bool;

private:
    ExporterGui                       _gui{};
    std::optional<VideoExportProcess> _video_export_process;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Exporter (Image and Video)", _gui)
        );
    }
};

} // namespace Cool