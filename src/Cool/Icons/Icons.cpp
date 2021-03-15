#include "Icons.h"

#include <Cool/LoadImage/LoadImage.h>
#include <Cool/OpenGL/Texture.h>

namespace Cool {

GLuint Icons::m_playID  = -1;
GLuint Icons::m_pauseID = -1;

void Icons::_LoadAll() {
	m_playID  = Texture::LoadTexture("Cool/Icons/icons/play.png");
	m_pauseID = Texture::LoadTexture("Cool/Icons/icons/pause.png");
}

void Icons::_DestroyAll() {
	Texture::DestroyTexture(m_playID);
	Texture::DestroyTexture(m_pauseID);
}

} // namespace Cool