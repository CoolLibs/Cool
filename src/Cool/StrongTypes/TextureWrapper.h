#pragma once
#include <Cool/Gpu/Texture.h>
#include <memory>
#include "Cool/Log/MessageId.h"

namespace Cool {

class TextureWrapper {
public:
    TextureWrapper()                                             = default;
    TextureWrapper(TextureWrapper&&) noexcept                    = default;
    auto operator=(TextureWrapper&&) noexcept -> TextureWrapper& = default;
    TextureWrapper(TextureWrapper const& o)
        : _absolute_path{o._absolute_path}
        , _texture{o._texture}
    {}
    auto operator=(TextureWrapper const& o) -> TextureWrapper&
    {
        if (&o != this)
        {
            _absolute_path = o._absolute_path;
            _texture       = o._texture;
        }
        return *this;
    }

    auto imgui_widget() -> bool;

    [[nodiscard]] auto texture() const -> Texture const&;

    friend auto operator==(TextureWrapper const& a, TextureWrapper const& b) -> bool { return a._absolute_path == b._absolute_path; }

private:
    void try_load_texture_from_path();

private:
    std::filesystem::path    _absolute_path{};
    std::shared_ptr<Texture> _texture{}; // We need a shared_ptr to make the class copyable
    MessageId                _error_id{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Path", _absolute_path));
        try_load_texture_from_path();
    }
};

} // namespace Cool