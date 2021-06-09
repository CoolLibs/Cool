#ifdef __COOL_APP_OPENGL

#include "../Texture.h"

#include <Cool/LoadImage/LoadImage.h>

namespace Cool {

Texture::~Texture() {
	DestroyTexture(m_textureID);
}

GLuint Texture::CreateTextureID(GLint interpolationMode, GLint wrapMode) {
	GLuint textureID;
	GLDebug(glGenTextures(1, &textureID));
	GLDebug(glBindTexture(GL_TEXTURE_2D, textureID));
	GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode));
	GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode));
	GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
	GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
	GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
	return textureID;
}

void Texture::DestroyTexture(GLuint texID) {
	GLDebug(glDeleteTextures(1, &texID));
}

Texture::Texture(std::string_view filepath, GLint interpolationMode, GLint wrapMode) 
	: m_textureID(LoadTexture(filepath, interpolationMode, wrapMode))
{
#ifndef NDEBUG
	m_bDataUploaded = true;
#endif
}

GLuint Texture::LoadTexture(std::string_view filepath, GLint interpolationMode, GLint wrapMode) {
	// Load image
	int width, height;
	std::unique_ptr<unsigned char> data = LoadImage::Load(filepath, &width, &height);
	// Create texture
	GLuint texID = Texture::CreateTextureID(interpolationMode, wrapMode);
	// Upload data
	GLDebug(glBindTexture(GL_TEXTURE_2D, texID));
	GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get()));
	//
	return texID;
}

void Texture::genTexture(GLint interpolationMode, GLint wrapMode) {
#ifndef NDEBUG
	if (m_textureID != -1)
		Log::error("[Texture::genTexture] You have already generated that texture !");
#endif
	m_textureID = CreateTextureID(interpolationMode, wrapMode);
}

void Texture::uploadRGB(int width, int height, unsigned char* data) {
#ifndef NDEBUG
	m_bDataUploaded = true;
	if (m_textureID == -1)
		Log::error("[Texture::uploadRGB] You haven't generated that texture yet !");
#endif
	GLDebug(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
	GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::uploadRGBA(int width, int height, unsigned char* data) {
#ifndef NDEBUG
	m_bDataUploaded = true;
	if (m_textureID == -1)
		Log::error("[Texture::uploadRGBA] You haven't generated that texture yet !");
#endif
	GLDebug(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::attachToSlot(int slot) {
#ifndef NDEBUG
	if (m_textureID == -1)
		Log::error("[Texture::attachToSlot] You haven't generated that texture yet !");
	if (!m_bDataUploaded)
		Log::error("[Texture::attachToSlot] You must upload some data (at least a width and height) before using the texture.");
#endif
	GLDebug(glActiveTexture(GL_TEXTURE0 + slot));
	GLDebug(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

} // namespace Cool

#endif