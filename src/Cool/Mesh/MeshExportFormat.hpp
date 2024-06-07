#pragma once

namespace Cool {

// TODO(Mesh) Add more supported export formats (obj, gltf, etc.)
enum class MeshExportFormat {
    ply,
};

[[nodiscard]] auto enum_name(MeshExportFormat export_format) -> std::string;
auto               imgui_combo(const char* label, MeshExportFormat& export_format) -> bool;

} // namespace Cool