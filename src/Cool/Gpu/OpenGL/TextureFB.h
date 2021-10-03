#pragma once
#if defined(COOL_OPENGL)

#include "FrameBuffer.h"

namespace Cool {

class TextureFB : public FrameBuffer {
public:
    TextureFB() = default;
    ~TextureFB();

    void   attachTextureToSlot(unsigned int slot);
    GLuint textureID() const { return m_colorTextureId; }

private:
    void createAttachments(ImageSize size) override;
    void destroyAttachments() override;
    void attachAttachments() override;

private:
    GLuint m_colorTextureId = -1;
};

} // namespace Cool

#endif