#include "Texture.h"

namespace Cool {

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_textureID));
}

void Texture::genTexture(GLint interpolationMode, GLint wrapMode) {
#ifndef DEBUG
	if (m_textureID != -1)
		Log::Error("[Texture::genTexture] You have already generated that texture !");
#endif
	GLCall(glGenTextures(1, &m_textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::uploadRGB(int width, int height, unsigned char* data) {
#ifndef NDEBUG
	m_bDataUploaded = true;
	if (m_textureID == -1)
		Log::Error("[Texture::uploadRGB] You haven't generated that texture yet !");
#endif
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::uploadRGBA(int width, int height, unsigned char* data) {
#ifndef NDEBUG
	m_bDataUploaded = true;
	if (m_textureID == -1)
		Log::Error("[Texture::uploadRGBA] You haven't generated that texture yet !");
#endif
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::bindToSlot(int slot) {
#ifndef NDEBUG
	if (m_textureID == -1)
		Log::Error("[Texture::bindToSlot] You haven't generated that texture yet !");
	if (!m_bDataUploaded)
		Log::Error("[Texture::bindToSlot] You must upload some data (at least a width and height) before using the texture.");
#endif
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

} // namespace Cool