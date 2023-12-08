#pragma once
#include <glpp/UniqueHandles/UniqueRenderBuffer.h>
#if defined(COOL_OPENGL)

#include "FrameBuffer.h"

namespace Cool {

class RenderBufferFB : public FrameBuffer {
public:
    RenderBufferFB() = default;

private:
    void createAttachments(img::Size size) override;
    void destroyAttachments() override;
    void attachAttachments() override;

private:
    std::optional<glpp::UniqueRenderbuffer> m_colorRenderBufferId{};
};

} // namespace Cool

#endif