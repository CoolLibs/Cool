#include <glpp/UniqueHandles/UniqueRenderBuffer.h>
#if defined(COOL_OPENGL)

#include "RenderBufferFB.h"

namespace Cool {

void RenderBufferFB::createAttachments(img::Size size)
{
    FrameBuffer::createAttachments(size);
    m_colorRenderBufferId.emplace();
    GLDebug(glBindRenderbuffer(GL_RENDERBUFFER, m_colorRenderBufferId->id()));
    GLDebug(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, static_cast<GLsizei>(size.width()), static_cast<GLsizei>(size.height())));
    GLDebug(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void RenderBufferFB::destroyAttachments()
{
    FrameBuffer::destroyAttachments();
    m_colorRenderBufferId.reset();
}

void RenderBufferFB::attachAttachments()
{
    FrameBuffer::attachAttachments();
    GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId()));
    GLDebug(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorRenderBufferId->id()));
    GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

} // namespace Cool

#endif