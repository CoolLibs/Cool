#pragma once
#include "img/src/Image.h"
#include "img/src/Size.h"

namespace Cool {

enum class Interpolation {
    NearestNeighbour,
    Linear,
    LinearMipmapLinear,
};

enum class Wrap {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
};

struct TextureConfig {
    Interpolation interpolation_mode{Interpolation::Linear};
    Wrap          wrap_mode{Wrap::ClampToEdge};
};

enum class InternalFormat {
    RGB,
    RGBA,
    // R                                  = GL_RED,
    // RG                                 = GL_RG,
    // RGB                                = GL_RGB,
    // RGBA                               = GL_RGBA,
    // DepthComponent                     = GL_DEPTH_COMPONENT,
    // DepthStencil                       = GL_DEPTH_STENCIL,
    // R8                                 = GL_R8,
    // R8_SNORM                           = GL_R8_SNORM,
    // R16                                = GL_R16,
    // R16_SNORM                          = GL_R16_SNORM,
    // RG8                                = GL_RG8,
    // RG8_SNORM                          = GL_RG8_SNORM,
    // RG16                               = GL_RG16,
    // RG16_SNORM                         = GL_RG16_SNORM,
    // R3_G3_B2                           = GL_R3_G3_B2,
    // RGB4                               = GL_RGB4,
    // RGB5                               = GL_RGB5,
    // RGB8                               = GL_RGB8,
    // RGB8_SNORM                         = GL_RGB8_SNORM,
    // RGB10                              = GL_RGB10,
    // RGB12                              = GL_RGB12,
    // RGB16_SNORM                        = GL_RGB16_SNORM,
    // RGBA2                              = GL_RGBA2,
    // RGBA4                              = GL_RGBA4,
    // RGB5_A1                            = GL_RGB5_A1,
    // RGBA8                              = GL_RGBA8,
    // RGBA8_SNORM                        = GL_RGBA8_SNORM,
    // RGB10_A2                           = GL_RGB10_A2,
    // RGB10_A2UI                         = GL_RGB10_A2UI,
    // RGBA12                             = GL_RGBA12,
    // RGBA16                             = GL_RGBA16,
    // SRGB8                              = GL_SRGB8,
    // SRGB8_ALPHA8                       = GL_SRGB8_ALPHA8,
    // R16F                               = GL_R16F,
    // RG16F                              = GL_RG16F,
    // RGB16F                             = GL_RGB16F,
    // RGBA16F                            = GL_RGBA16F,
    // R32F                               = GL_R32F,
    // RG32F                              = GL_RG32F,
    // RGB32F                             = GL_RGB32F,
    // RGBA32F                            = GL_RGBA32F,
    // R11F_G11F_B10F                     = GL_R11F_G11F_B10F,
    // RGB9_E5                            = GL_RGB9_E5,
    // R8I                                = GL_R8I,
    // R8UI                               = GL_R8UI,
    // R16I                               = GL_R16I,
    // R16UI                              = GL_R16UI,
    // R32I                               = GL_R32I,
    // R32UI                              = GL_R32UI,
    // RG8I                               = GL_RG8I,
    // RG8UI                              = GL_RG8UI,
    // RG16I                              = GL_RG16I,
    // RG16UI                             = GL_RG16UI,
    // RG32I                              = GL_RG32I,
    // RG32UI                             = GL_RG32UI,
    // RGB8I                              = GL_RGB8I,
    // RGB8UI                             = GL_RGB8UI,
    // RGB16I                             = GL_RGB16I,
    // RGB16UI                            = GL_RGB16UI,
    // RGB32I                             = GL_RGB32I,
    // RGB32UI                            = GL_RGB32UI,
    // RGBA8I                             = GL_RGBA8I,
    // RGBA8UI                            = GL_RGBA8UI,
    // RGBA16I                            = GL_RGBA16I,
    // RGBA16UI                           = GL_RGBA16UI,
    // RGBA32I                            = GL_RGBA32I,
    // RGBA32UI                           = GL_RGBA32UI,
    // Compressed_R                       = GL_COMPRESSED_RED,
    // Compressed_RG                      = GL_COMPRESSED_RG,
    // Compressed_RGB                     = GL_COMPRESSED_RGB,
    // Compressed_RGBA                    = GL_COMPRESSED_RGBA,
    // Compressed_SRGB                    = GL_COMPRESSED_SRGB,
    // Compressed_SRGB_ALPHA              = GL_COMPRESSED_SRGB_ALPHA,
    // Compressed_RED_RGTC1               = GL_COMPRESSED_RED_RGTC1,
    // Compressed_SIGNED_RED_RGTC1        = GL_COMPRESSED_SIGNED_RED_RGTC1,
    // Compressed_RG_RGTC2                = GL_COMPRESSED_RG_RGTC2,
    // Compressed_SIGNED_RG_RGTC2         = GL_COMPRESSED_SIGNED_RG_RGTC2,
    // Compressed_RGBA_BPTC_UNORM         = GL_COMPRESSED_RGBA_BPTC_UNORM,
    // Compressed_SRGB_ALPHA_BPTC_UNORM   = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
    // Compressed_RGB_BPTC_SIGNED_FLOAT   = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
    // Compressed_RGB_BPTC_UNSIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,

};

enum class Channels {
    RGB,
    RGBA,
    // R              = GL_RED,
    // RG             = GL_RG,
    // RGB            = GL_RGB,
    // BGR            = GL_BGR,
    // RGBA           = GL_RGBA,
    // BGRA           = GL_BGRA,
    // R_Integer      = GL_RED_INTEGER,
    // RG_Integer     = GL_RG_INTEGER,
    // RGB_Integer    = GL_RGB_INTEGER,
    // BGR_Integer    = GL_BGR_INTEGER,
    // RGBA_Integer   = GL_RGBA_INTEGER,
    // BGRA_Integer   = GL_BGRA_INTEGER,
    // StencilIndex   = GL_STENCIL_INDEX,
    // DepthComponent = GL_DEPTH_COMPONENT,
    // DepthStencil   = GL_DEPTH_STENCIL,
};

enum class TexelDataType {
    UnsignedByte,
    // UnsignedByte               = GL_UNSIGNED_BYTE,
    // Byte                       = GL_BYTE,
    // UnsignedShort              = GL_UNSIGNED_SHORT,
    // Short                      = GL_SHORT,
    // UnsignedInt                = GL_UNSIGNED_INT,
    // Int                        = GL_INT,
    // Float                      = GL_FLOAT,
    // UnsignedByte_3_3_2         = GL_UNSIGNED_BYTE_3_3_2,
    // UnsignedByte_2_3_3_Rev     = GL_UNSIGNED_BYTE_2_3_3_REV,
    // UnsignedShort_5_6_5        = GL_UNSIGNED_SHORT_5_6_5,
    // UnsignedShort_5_6_5_Rev    = GL_UNSIGNED_SHORT_5_6_5_REV,
    // UnsignedShort_4_4_4_4      = GL_UNSIGNED_SHORT_4_4_4_4,
    // UnsignedShort_4_4_4_4_Rev  = GL_UNSIGNED_SHORT_4_4_4_4_REV,
    // UnsignedShort_5_5_5_1      = GL_UNSIGNED_SHORT_5_5_5_1,
    // UnsignedShort_1_5_5_5_Rev  = GL_UNSIGNED_SHORT_1_5_5_5_REV,
    // UnsignedInt_8_8_8_8        = GL_UNSIGNED_INT_8_8_8_8,
    // UnsignedInt_8_8_8_8_Rev    = GL_UNSIGNED_INT_8_8_8_8_REV,
    // UnsignedInt_10_10_10_2     = GL_UNSIGNED_INT_10_10_10_2,
    // UnsignedInt_2_10_10_10_Rev = GL_UNSIGNED_INT_2_10_10_10_REV,
};

struct TextureLayout {
    InternalFormat internal_format = InternalFormat::RGBA;
    Channels       channels        = Channels::RGBA;
    TexelDataType  texel_data_type = TexelDataType::UnsignedByte;
};

class Texture {
public:
    /// Creates an empty texture.
    explicit Texture(TextureConfig = {});
    /// Create a texture with the given size and no pixel data.
    explicit Texture(img::Size const&, TextureConfig = {});
    /// Create a texture containing a copy of the image's data.
    explicit Texture(img::Image const&, TextureConfig = {});
    /// Create a texture containing a copy of data.
    /// `channels_count` should be 3 for RGB and 4 for RGBA.
    Texture(img::Size const&, int channels_count, uint8_t const* data, TextureConfig = {});
    Texture(img::Size const&, uint8_t const* data, TextureLayout const&, TextureConfig = {});

    void set_size(img::Size const&);
    void set_image(img::Image const&);
    /// `channels_count` should be 3 for RGB and 4 for RGBA.
    void set_image(img::Size const&, int channels_count, uint8_t const* data);
    void set_image(img::Size const&, uint8_t const* data, TextureLayout const&);
    void set_interpolation_mode(Interpolation interpolation_mode);
    void set_wrap_mode(Wrap wrap_mode);

    void        bind();
    static void unbind();

    /// The native WebGPU of the texture.
    [[nodiscard]] auto id() const /* -> GLuint */ { return 0; } // TODO(WebGPU)
    /// The ID that ImGui expects
    [[nodiscard]] auto imgui_texture_id() const -> ImTextureID { return reinterpret_cast<ImTextureID>(static_cast<uint64_t>(id())); } // Double-cast to fix a warning : first we convert to the correct size (uint32_t -> uint64_t) then from integral type to pointer type (uint64_t -> ImTextureID)

    [[nodiscard]] auto aspect_ratio() const -> float { return _aspect_ratio; }

    /// Attaches your texture to a slot, so that it is ready to be read by a shader.
    /// This `slot` is the value that you should also send to your shader as the value of a "uniform sampler2D u_YourTextureUniform"
    // void attach_to_slot(GLuint slot) const;

private:
    // glpp::Texture2D _tex{};
    float _aspect_ratio{};
#if DEBUG
    bool _data_has_been_uploaded{false};
#endif
};

} // namespace Cool