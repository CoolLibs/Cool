#pragma once

namespace Cool {

class Texture {
public:
    struct Id {
        operator ImTextureID() const
        {
            return reinterpret_cast<ImTextureID>(id);
        }
#if defined(__COOL_APP_OPENGL)
        GLuint id;
#elif defined(__COOL_APP_VULKAN)
        unsigned int id; // TODO
#endif
    };

    static Id LoadTexture(std::string_view path)
    {
        return {0};
    }
};

} // namespace Cool