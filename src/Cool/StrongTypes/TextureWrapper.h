#pragma once
#include <Cool/Gpu/Texture.h>
#include <memory>
#include "Cool/Log/MessageId.h"

namespace Cool {

enum class TextureRepeatMode {
    None,
    Mirror,
    Mosaic,
    Clamp
};

class TextureWrapper {
public:
    TextureWrapper()                                             = default;
    TextureWrapper(TextureWrapper&&) noexcept                    = default;
    auto operator=(TextureWrapper&&) noexcept -> TextureWrapper& = default;
    TextureWrapper(TextureWrapper const& o)
        : _absolute_path{o._absolute_path}
        , _texture{o._texture}
        , _repeat_mode{o._repeat_mode}
    {}
    auto operator=(TextureWrapper const& o) -> TextureWrapper&
    {
        if (&o != this)
        {
            _absolute_path = o._absolute_path;
            _texture       = o._texture;
            _repeat_mode   = o._repeat_mode;
        }
        return *this;
    }

    auto imgui_widget() -> bool;

    [[nodiscard]] auto texture() const -> Texture const&;
    [[nodiscard]] auto aspect_ratio() const -> float { return texture().aspect_ratio(); }

    friend auto operator==(TextureWrapper const& a, TextureWrapper const& b) -> bool { return a._absolute_path == b._absolute_path; }

private:
    void try_load_texture_from_path();
    void apply_repeat_mode();

private:
    std::filesystem::path    _absolute_path{};
    std::shared_ptr<Texture> _texture{}; // We need a shared_ptr to make the class copyable
    TextureRepeatMode        _repeat_mode{TextureRepeatMode::None};
    MessageId                _error_id{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Path", _absolute_path),
            cereal::make_nvp("Repeat Mode", _repeat_mode)
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(_absolute_path);
        try_load_texture_from_path();
    }
};

} // namespace Cool