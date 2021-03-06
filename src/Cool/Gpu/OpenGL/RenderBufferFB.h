#pragma once
#if defined(COOL_OPENGL)

#include "FrameBuffer.h"

namespace Cool {

class RenderBufferFB : public FrameBuffer {
public:
    RenderBufferFB() = default;
    ~RenderBufferFB();

private:
    void createAttachments(img::Size size) override;
    void destroyAttachments() override;
    void attachAttachments() override;

private:
    GLuint m_colorRenderBufferId = static_cast<GLuint>(-1);
};

} // namespace Cool

#endif