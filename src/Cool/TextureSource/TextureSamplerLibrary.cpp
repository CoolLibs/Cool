#include "TextureSamplerLibrary.h"
#include "TextureSamplerDescriptor.h"
#include "glpp/UniqueHandles/UniqueTextureSampler.h"

namespace Cool {

static void apply_repeat_mode(GLuint sampler, TextureRepeatMode mode)
{
    switch (mode)
    {
    case TextureRepeatMode::None:
    {
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));
        GLfloat color[4] = {0.f, 0.f, 0.f, 0.f};                                // NOLINT(*-avoid-c-arrays)
        GLDebug(glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, color)); // TODO(JF) Wrap into glpp
        break;
    }
    case TextureRepeatMode::Mirror:
    {
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT));
        break;
    }
    case TextureRepeatMode::Mosaic:
    {
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_REPEAT));
        break;
    }
    case TextureRepeatMode::Clamp:
    {
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLDebug(glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        break;
    }
    }
}

auto TextureSamplerLibrary::get(TextureSamplerDescriptor const& info) -> glpp::UniqueTextureSampler const&
{
    // Try to find an existing sampler with the same info
    auto const it = _samplers.find(info);
    if (it != _samplers.end())
        return it->second;

    // Create a new sampler
    _samplers[info] = glpp::UniqueTextureSampler{};
    auto& sampler   = _samplers[info];
    apply_repeat_mode(sampler.id(), info.repeat_mode);
    glSamplerParameteri(sampler.id(), GL_TEXTURE_MAG_FILTER, glpp::raw(info.interpolation_mode));
    glSamplerParameteri(sampler.id(), GL_TEXTURE_MIN_FILTER, glpp::raw(info.interpolation_mode));

    return sampler;
}

} // namespace Cool