#pragma once
#include "MeshExportFormat.hpp"

namespace Cool {

class MeshExportSettings {
public:
    std::filesystem::path path{"mesh(0).ply"};
    auto                  format() const -> MeshExportFormat;

    void set_file_name_to_an_unused_name();
    auto imgui() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Path", path)
        );
    }
};

} // namespace Cool