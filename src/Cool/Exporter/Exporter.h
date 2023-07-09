#pragma once
#include "ExporterGui.h"
#include "VideoExportProcess.h"

namespace Cool {

class Exporter {
public:
    [[nodiscard]] auto is_exporting() const -> bool { return Cool::ExporterGui::is_exporting(_video_export_process); }

    auto image_export_window() -> ImGuiWindow& { return _gui.image_export_window(); }
    auto video_export_window() -> ImGuiWindow& { return _gui.video_export_window(); }

    void set_aspect_ratio(AspectRatio const& aspect_ratio) { _gui.set_aspect_ratio(aspect_ratio); }
    void maybe_set_aspect_ratio(std::optional<AspectRatio> const& aspect_ratio) { _gui.maybe_set_aspect_ratio(aspect_ratio); }

    /// Displays all the currently active windows.
    void imgui_windows(Polaroid const& polaroid, float time) { _gui.imgui_windows(polaroid, time, _video_export_process); }

    /// The buttons to open the different exporter windows.
    void imgui_menu_items(imgui_menu_items_Params const& params = {}, std::optional<std::string> const& longest_text = {}) { _gui.imgui_menu_items(params, longest_text); }

    /// Call this after your rendering code. If we are exporting it will export the current frame and decide if the export should continue.
    void update(Polaroid const& polaroid) { _gui.update(polaroid, _video_export_process); }

private:
    ExporterGui                       _gui;
    std::optional<VideoExportProcess> _video_export_process;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Exporter (Image and Video)", _gui)
        );
    }
};

} // namespace Cool