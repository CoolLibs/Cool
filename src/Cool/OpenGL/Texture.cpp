#include "Texture.h"

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_textureID));
}

void Texture::genTexture(GLint interpolationMode, GLint wrapMode) {
	GLCall(glGenTextures(1, &m_textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::uploadRGB(int width, int height, unsigned char* data) {
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::uploadRGBA(int width, int height, unsigned char* data) {
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::bindToSlot(int slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture::unbind() {
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}