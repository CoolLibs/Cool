#pragma once
#ifdef __COOL_APP_OPENGL

#include <Cool/OpenGL/RenderBufferFB.h>

namespace Cool {

/// <summary>
/// A simple class that can only make fullscreen draw calls. By the way, it uses a very nice trick explained here : https://stackoverflow.com/a/59739538 
/// </summary>
class Renderer_Fullscreen {
public:
	Renderer_Fullscreen();
	~Renderer_Fullscreen();
	Renderer_Fullscreen(const Renderer_Fullscreen& other) = delete;			   // non-copyable because it would destroy the VAO (we would need move operators)
	Renderer_Fullscreen& operator=(const Renderer_Fullscreen& other) = delete; // non-copyable because it would destroy the VAO (we would need move operators)

	/// <summary>
	/// Call this before doing any rendering code. It sets up the RenderBuffer that we will render to.
	/// </summary>
	void begin();
	/// <summary>
	/// Call this after all your rendering code. It displays in the viewing area what we have rendered.
	/// </summary>
	void end();
	/// <summary>
	/// Draws on the whole screen : your fragment shader will be called for every pixel of the screen / render area.
	/// </summary>
	void render();
	/// <summary>
	/// The RenderBuffer that we render to. You can grab it and do advanced stuff with it if you know what you are doing.
	/// </summary>
	inline RenderBufferFB& renderBuffer() { return m_renderBuffer; }

private:
	void onRenderAreaResized();

private:
	RenderBufferFB m_renderBuffer;
	GLuint m_dummyVaoID;
};

} // namespace Cool

#endif