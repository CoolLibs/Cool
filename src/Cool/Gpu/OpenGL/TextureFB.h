#pragma once
#if defined(COOL_OPENGL)
#include "FrameBuffer.h"
#include "glpp/glpp.hpp"

namespace Cool {

class TextureFB : public FrameBuffer {
public:
    TextureFB() = default;

    void   attachTextureToSlot(unsigned int slot) const;
    GLuint textureID() const { return m_colorTextureId->id(); }

private:
    void createAttachments(img::Size size) override;
    void destroyAttachments() override;
    void attachAttachments() override;

private:
    std::optional<glpp::UniqueTexture2D> m_colorTextureId{};
};

} // namespace Cool

#endif