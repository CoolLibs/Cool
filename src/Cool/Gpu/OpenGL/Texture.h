#pragma once
#include "glpp-extended/src/Texture2D.h"
#if defined(COOL_OPENGL)

#include <glpp/glpp.hpp>
#include "img/src/Image.h"
#include "img/src/Size.h"

namespace Cool::OpenGL {

/// A wrapper for an OpenGL texture2D
class Texture {
public:
    struct Config {
        glpp::Interpolation interpolation_mode{glpp::Interpolation::Linear};
        glpp::Wrap          wrap_mode{glpp::Wrap::ClampToEdge};

        Config(){}; // NOLINT(*-use-equals-default) Need to declare a non-defaulted default constructor to work around a bug in Clang
    };
    /// Creates an empty texture.
    explicit Texture(Config = {});
    /// Create a texture with the given size and no pixel data.
    explicit Texture(img::Size const&, Config = {});
    /// Create a texture containing a copy of the image's data.
    explicit Texture(img::Image const&, Config = {});

    void set_size(img::Size const&);
    void set_image(img::Image const&);
    void set_interpolation_mode(glpp::Interpolation interpolation_mode);
    void set_wrap_mode(glpp::Wrap wrap_mode);

    void        bind();
    static void unbind();

    /// The native OpenGL ID of the texture.
    [[nodiscard]] auto id() const -> GLuint { return *_tex; }
    /// The ID that ImGui expects
    [[nodiscard]] auto imgui_texture_id() const -> ImTextureID { return reinterpret_cast<ImTextureID>(static_cast<uint64_t>(id())); } // Double-cast to fix a warning : first we convert to the correct size (uint32_t -> uint64_t) then from integral type to pointer type (uint64_t -> ImTextureID)

    [[nodiscard]] auto aspect_ratio() const -> float { return _aspect_ratio; }

    /// <summary>
    /// Attaches your texture to a slot, so that it is ready to be read by a shader.
    /// </summary>
    /// <param name="slot">The slot. This should match the "uniform sampler2D u_TextureSlot" in your shader that is set through set_uniform(slot)</param>
    void attach_to_slot(int slot) const;

private:
    glpp::Texture2D _tex{};
    float           _aspect_ratio{};
#if DEBUG
    bool _data_has_been_uploaded{false};
#endif
};

} // namespace Cool::OpenGL

#endif