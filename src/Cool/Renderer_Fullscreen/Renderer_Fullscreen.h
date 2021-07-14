#pragma once
#ifdef __COOL_APP_OPENGL

    #include <Cool/OpenGL/RenderBufferFB.h>

namespace Cool {

/**
 * @brief A simple class that can only make fullscreen draw calls. By the way, it uses a very nice trick explained here : https://stackoverflow.com/a/59739538 
 * 
 */
class Renderer_Fullscreen {
public:
    Renderer_Fullscreen();
    ~Renderer_Fullscreen();
    Renderer_Fullscreen(const Renderer_Fullscreen& other) = delete;            // non-copyable because it would destroy the VAO (we would need move operators)
    Renderer_Fullscreen& operator=(const Renderer_Fullscreen& other) = delete; // non-copyable because it would destroy the VAO (we would need move operators)

    /**
	 * @brief Call this before doing any rendering code. It sets up the RenderBuffer that we will render to.
	 * 
	 */
    void begin();

    /**
	 * @brief Call this after all your rendering code. It displays in the viewing area what we have rendered.
	 * 
	 */
    void end();

    /**
	 * @brief Draws on the whole screen : your fragment shader will be called for every pixel of the screen / render area.
	 * 
	 */
    void render();

    /**
	 * @brief The RenderBuffer that we render to. You can grab it and do advanced stuff with it if you know what you are doing.
	 * 
	 * @return RenderBufferFB& 
	 */
    inline RenderBufferFB& renderBuffer() { return _render_buffer; }

private:
    void on_render_area_resized();

private:
    RenderBufferFB _render_buffer;
    GLuint         _dummy_vao_id;
};

} // namespace Cool

#endif