#pragma once
#if defined(COOL_OPENGL)

namespace Cool::OpenGL {

/// <summary>
/// A wrapper for an OpenGL texture2D
/// </summary>
class Texture {
public:
    Texture() = default;
    ~Texture();
    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    Texture(const Texture&) = delete;            // Non-copyable
    Texture& operator=(const Texture&) = delete; // Non-copyable

    ImTextureID imgui_texture_id() const { return reinterpret_cast<ImTextureID>(static_cast<uint64_t>(m_textureID)); } // Double-cast to fix a warning : first we convert to the correct size (uint32_t -> uint64_t) then from integral type to pointer type (uint64_t -> ImTextureID)

    /// <summary>
    /// Constructs a texture from an image.
    /// </summary>
    /// <param name="filepath">The path to the image to be loaded in the texture</param>
    /// <param name="interpolationMode">The OpenGL interpolation mode (how the texture will be resized if needed).
    /// The default value is GL_LINEAR.
    /// Can be either GL_LINEAR or GL_NEAREST (nearest neighbour, a.k.a. no real interpolation)
    /// </param>
    /// <param name="wrapMode">The OpenGL wrap mode (how the texture will be sampled if you look at coordinates outside the [0, 1] range).
    /// The default value is GL_CLAMP_TO_EDGE.
    /// Can be either GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE (one repetition and then we clamp) or GL_CLAMP_TO_BORDER (like clamp, but uses a fixed color instead of the color at the edge of the texture)
    /// </param>
    Texture(std::string_view filepath, GLint interpolationMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);

    /// <summary>
    /// Loads an image into a texture and returns the OpenGL ID. You are then responsible for destroying the texture with Texture::DestroyTexture()
    /// </summary>
    /// <param name="filepath">The path to the image to be loaded</param>
    /// <param name="interpolationMode">The OpenGL interpolation mode (how the texture will be resized if needed).
    /// The default value is GL_LINEAR.
    /// Can be either GL_LINEAR or GL_NEAREST (nearest neighbour, a.k.a. no real interpolation)
    /// </param>
    /// <param name="wrapMode">The OpenGL wrap mode (how the texture will be sampled if you look at coordinates outside the [0, 1] range).
    /// The default value is GL_CLAMP_TO_EDGE.
    /// Can be either GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE (one repetition and then we clamp) or GL_CLAMP_TO_BORDER (like clamp, but uses a fixed color instead of the color at the edge of the texture)
    /// </param>
    /// <returns></returns>
    static GLuint LoadTexture(std::string_view filepath, GLint interpolationMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);

    /// <summary>
    /// Creates an OpenGL texture. You are then responsible for destroying it with Texture::DestroyTexture()
    /// </summary>
    /// <param name="interpolationMode">The OpenGL interpolation mode (how the texture will be resized if needed).
    /// The default value is GL_LINEAR.
    /// Can be either GL_LINEAR or GL_NEAREST (nearest neighbour, a.k.a. no real interpolation)
    /// </param>
    /// <param name="wrapMode">The OpenGL wrap mode (how the texture will be sampled if you look at coordinates outside the [0, 1] range).
    /// The default value is GL_CLAMP_TO_EDGE.
    /// Can be either GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE (one repetition and then we clamp) or GL_CLAMP_TO_BORDER (like clamp, but uses a fixed color instead of the color at the edge of the texture)
    /// </param>
    /// <returns>The OpenGL ID of the texture.</returns>
    static GLuint CreateTextureID(GLint interpolationMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);

    /// <summary>
    /// Destroys the OpenGL texture
    /// </summary>
    /// <param name="texID"></param>
    static void DestroyTexture(GLuint texID);

    /// <summary>
    /// Actually constructs the OpenGL texture.
    /// We don't do this in the constructor to allow the use of static textures (those would fail because OpenGL is only initialized once AppManager has been constructed)
    /// </summary>
    /// <param name="interpolationMode">The OpenGL interpolation mode (how the texture will be resized if needed).
    /// The default value is GL_LINEAR.
    /// Can be either GL_LINEAR or GL_NEAREST (nearest neighbour, a.k.a. no real interpolation)
    /// </param>
    /// <param name="wrapMode">The OpenGL wrap mode (how the texture will be sampled if you look at coordinates outside the [0, 1] range).
    /// The default value is GL_CLAMP_TO_EDGE.
    /// Can be either GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE (one repetition and then we clamp) or GL_CLAMP_TO_BORDER (like clamp, but uses a fixed color instead of the color at the edge of the texture)
    /// </param>
    void genTexture(GLint interpolationMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);

    /// <summary>
    /// Upload the image data on the GPU (or just the size if you plan on writting on the texture through shaders).
    /// </summary>
    /// <param name="width">Width in pixels of the image represented by data</param>
    /// <param name="height">Height in pixels of the image represented by data</param>
    /// <param name="data">An array of unsigned chars. You can leave as nullptr if you just want to set the size of the texture. 3 consecutive unsigned chars make a pixel (Red Green Blue).
    /// The OpenGL convention is that the first pixel in the array is the bottom-left of the image, then the second is on the first row second column and so on.</param>
    void uploadRGB(int width, int height, unsigned char* data = nullptr);

    /// <summary>
    /// Upload the image data on the GPU (or just the size if you plan on writting on the texture through shaders).
    /// </summary>
    /// <param name="width">Width in pixels of the image represented by data</param>
    /// <param name="height">Height in pixels of the image represented by data</param>
    /// <param name="data">An array of unsigned chars. You can leave as nullptr if you just want to set the size of the texture. 4 consecutive unsigned chars make a pixel (Red Green Blue Alpha).
    /// The OpenGL convention is that the first pixel in the array is the bottom-left of the image, then the second is on the first row second column and so on.</param>
    void uploadRGBA(int width, int height, unsigned char* data = nullptr);

    /// <summary>
    /// Attaches your texture to a slot, so that it is ready to be read by a shader.
    /// </summary>
    /// <param name="slot">The slot. This should match the "uniform sampler2D u_TextureSlot" in your shader that is set through set_uniform(slot)</param>
    void attachToSlot(int slot);

    /// <summary>
    /// The OpenGL ID of the texture.
    /// </summary>
    inline GLuint ID() { return m_textureID; }

private:
    GLuint m_textureID = -1;
#ifdef DEBUG
    bool m_bDataUploaded = false;
#endif
};

} // namespace Cool::OpenGL

#endif