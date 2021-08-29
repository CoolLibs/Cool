#ifdef __COOL_APP_OPENGL
#include "../TextureFB.h"

namespace Cool {

TextureFB::~TextureFB()
{
    destroyAttachments();
}

void TextureFB::createAttachments(ImageSize size)
{
    FrameBuffer::createAttachments(size);
    GLDebug(glGenTextures(1, &m_colorTextureId));
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
    GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size.width(), size.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
}

void TextureFB::destroyAttachments()
{
    FrameBuffer::destroyAttachments();
    GLDebug(glDeleteTextures(1, &m_colorTextureId));
}

void TextureFB::attachAttachments()
{
    FrameBuffer::attachAttachments();
    GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId()));
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
    GLDebug(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureId, 0));
    GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void TextureFB::attachTextureToSlot(unsigned int slot)
{
    GLDebug(glActiveTexture(GL_TEXTURE0 + slot));
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
}

} // namespace Cool

#endif