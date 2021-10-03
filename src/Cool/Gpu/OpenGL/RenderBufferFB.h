#pragma once
#if defined(COOL_OPENGL)

#include "FrameBuffer.h"

namespace Cool {

class RenderBufferFB : public FrameBuffer {
public:
    RenderBufferFB() = default;
    ~RenderBufferFB();

private:
    void createAttachments(ImageSize size) override;
    void destroyAttachments() override;
    void attachAttachments() override;

private:
    GLuint m_colorRenderBufferId = -1;
};

} // namespace Cool

#endif