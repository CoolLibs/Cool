#pragma once
#include <memory>
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

    auto image_export_window() -> ImGuiWindow& { return _gui->image_export_window(); }
    auto video_export_window() -> ImGuiWindow& { return _gui->video_export_window(); }

    void set_aspect_ratio(AspectRatio const& aspect_ratio) { _gui->set_aspect_ratio(aspect_ratio); }
    void maybe_set_aspect_ratio(std::optional<AspectRatio> const& aspect_ratio) { _gui->maybe_set_aspect_ratio(aspect_ratio); }

    /// Displays all the currently active windows.
    void imgui_windows(exporter_imgui_windows_Params const& p) { _gui->imgui_windows(p, _video_export_process); }

    /// The buttons to open the different exporter windows.
    void imgui_menu_items(exporter_imgui_menu_items_Params const& params = {}, std::optional<std::string> const& longest_text = {}) { _gui->imgui_menu_items(params, longest_text); }

    /// Call this after your rendering code. If we are exporting it will export the current frame and decide if the export should continue.
    void update(Polaroid const& polaroid) { _gui->update(polaroid, _video_export_process); }

    /// Can only be called if `is_exporting()` is true.
    auto clock() const -> Clock const&;

private:
    std::unique_ptr<ExporterGui>      _gui{std::make_unique<ExporterGui>()}; // Pointer so that the address doesn't change when we move the Exporter (important because we have events that store references to things in the gui)
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