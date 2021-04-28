#pragma once

#ifdef __COOL_APP_OPENGL

namespace Cool {

static constexpr GLuint SCREEN_FRAMEBUFFER_ID = 0;

class FrameBuffer {
public:
	FrameBuffer();
	virtual ~FrameBuffer();

	void setSize(const glm::ivec2& size);

	void bind();
	void unbind();
	/// <summary>
	/// Copies the content of this framebuffer to another framebuffer
	/// </summary>
	/// <param name="botLeft">Coordinates of the bottom left corner of the area to copy (in pixels)</param>
	/// <param name="topRight">Coordinates of the top right corner of the area to copy (in pixels)</param>
	/// <param name="dstFrameBufferID">OpenGL ID of the target framebuffer</param>
	/// <param name="interpolationMode">The OpenGL interpolation mode (how the image will be resized if needed).
	/// The default value is GL_LINEAR.
	/// Can be either GL_LINEAR or GL_NEAREST (nearest neighbour, a.k.a. no real interpolation)
	/// </param>
	void blitTo(const glm::ivec2& botLeft, const glm::ivec2& topRight, GLuint dstFrameBufferID = SCREEN_FRAMEBUFFER_ID, GLint interpolationMode = GL_LINEAR);
	/// <summary>
	/// Copies the content of this framebuffer to another framebuffer
	/// </summary>
	/// <param name="interpolationMode">The OpenGL interpolation mode (how the image will be resized if needed).
	/// The default value is GL_LINEAR.
	/// Can be either GL_LINEAR or GL_NEAREST (nearest neighbour, a.k.a. no real interpolation)
	/// </param>
	void blitTo(FrameBuffer& frameBuffer, GLint interpolationMode = GL_LINEAR);

	inline int width()  const { return m_size.x; }
	inline int height() const { return m_size.y; }
	inline const glm::ivec2& size() const { return m_size; }
	inline float aspectRatio() const { return static_cast<float>(m_size.x) / static_cast<float>(m_size.y); }

protected:
	virtual void destroyAttachments();
	virtual void createAttachments(int width, int height);
	virtual void attachAttachments();

	inline GLuint frameBufferId() { return m_frameBufferId; }

private:
	GLuint m_frameBufferId = -1;
	GLuint m_depthRenderBufferId = -1;
	glm::ivec2 m_size = glm::ivec2(0);
	int m_prevViewport[4];
};

} // namespace Cool

#endif