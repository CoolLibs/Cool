#include <glpp/UniqueHandles/UniqueTexture.h>
#if defined(COOL_OPENGL)

#include "TextureFB.h"

namespace Cool {

void TextureFB::createAttachments(img::Size size)
{
    FrameBuffer::createAttachments(size);
    m_colorTextureId.emplace();
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_colorTextureId->id()));
    GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(size.width()), static_cast<GLsizei>(size.height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));  // TODO should be nearest ?
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); // TODO should be linear ?
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
}

void TextureFB::destroyAttachments()
{
    FrameBuffer::destroyAttachments();
    m_colorTextureId.reset();
}

void TextureFB::attachAttachments()
{
    FrameBuffer::attachAttachments();
    GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId()));
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_colorTextureId->id()));
    GLDebug(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureId->id(), 0));
    GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void TextureFB::attachTextureToSlot(unsigned int slot) const
{
    GLDebug(glActiveTexture(GL_TEXTURE0 + slot));
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_colorTextureId->id()));
}

} // namespace Cool

#endif