#pragma once
#include <Cool/Gpu/Texture.h>
#include <memory>

namespace Cool {

class TextureWrapper {
public:
    auto imgui_widget() -> bool;

    [[nodiscard]] auto texture() const -> Texture const& { return *_texture; }

    friend auto operator==(TextureWrapper const& a, TextureWrapper const& b) -> bool { return a._absolute_path == b._absolute_path; }

private:
    std::filesystem::path    _absolute_path{};
    std::shared_ptr<Texture> _texture{}; // We need a shared_ptr to make the class copyable

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Path", _absolute_path));
    }
};

} // namespace Cool