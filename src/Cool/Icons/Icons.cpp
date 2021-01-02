#include "Icons.h"

#include <Cool/LoadImage/LoadImage.h>
#include <Cool/OpenGL/Texture.h>

namespace Cool {

GLuint Icons::m_playID  = -1;
GLuint Icons::m_pauseID = -1;

void Icons::_LoadAll() {
	m_playID  = LoadTexture("Cool/Icons/icons/play.png");
	m_pauseID = LoadTexture("Cool/Icons/icons/pause.png");
}

void Icons::_DestroyAll() {
	DestroyTexture(m_playID);
	DestroyTexture(m_pauseID);
}

GLuint Icons::LoadTexture(const char* filepath) {
	// Load image
	int width, height;
	unsigned char* data = LoadImage::Load(filepath, &width, &height, false);
	// Create texture
	GLuint texID = Texture::CreateTextureID(GL_LINEAR, GL_CLAMP_TO_EDGE);
	// Upload data
	GLCall(glBindTexture(GL_TEXTURE_2D, texID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	// Cleanup
	LoadImage::Free(data);
	//
	return texID;
}

void Icons::DestroyTexture(unsigned int texID) {
	GLCall(glDeleteTextures(1, &texID));
}

} // namespace Cool