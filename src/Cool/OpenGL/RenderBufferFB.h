#pragma once

#include "FrameBuffer.h"

namespace Cool {

class RenderBufferFB : public FrameBuffer {
public:
	RenderBufferFB() = default;
	~RenderBufferFB();

private:
	void createAttachments(int width, int height) override;
	void destroyAttachments() override;
	void attachAttachments() override;

private:
	GLuint m_colorRenderBufferId = -1;
};

} // namespace Cool