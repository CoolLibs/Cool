#include "../Texture.h"

namespace Cool {

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_textureID));
}

GLuint Texture::CreateTextureID(GLint interpolationMode, GLint wrapMode) {
	GLuint textureID;
	GLCall(glGenTextures(1, &textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	return textureID;
}

void Texture::genTexture(GLint interpolationMode, GLint wrapMode) {
#ifndef DEBUG
	if (m_textureID != -1)
		Log::Error("[Texture::genTexture] You have already generated that texture !");
#endif
	m_textureID = CreateTextureID(interpolationMode, wrapMode);
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

void Texture::attachToSlot(int slot) {
#ifndef NDEBUG
	if (m_textureID == -1)
		Log::Error("[Texture::attachToSlot] You haven't generated that texture yet !");
	if (!m_bDataUploaded)
		Log::Error("[Texture::attachToSlot] You must upload some data (at least a width and height) before using the texture.");
#endif
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

} // namespace Cool