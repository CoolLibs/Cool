#pragma once

namespace Cool {

class Texture {
public:
	Texture() = default;
	~Texture();
	Texture(const Texture&) = delete;			 // Non-copyable
	Texture(Texture&&) = delete;				 // Non-copyable
	Texture& operator=(const Texture&) = delete; // Non-copyable

	void genTexture(GLint interpolationMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
	void uploadRGB (int width, int height, unsigned char* data = nullptr);
	void uploadRGBA(int width, int height, unsigned char* data = nullptr);

	void bindToSlot(int slot);
	void unbind();

	inline GLuint ID() { return m_textureID; }

private:
	GLuint m_textureID = -1;
#ifndef NDEBUG
	bool m_bDataUploaded = false;
#endif
};

} // namespace Cool