#if defined(COOL_OPENGL)

#include "../FullscreenPipeline.h"
#include <Cool/File/File.h>

namespace Cool::OpenGL {

FullscreenPipeline::FullscreenPipeline(std::string_view fragment_shader_path)
    : _shader{File::root_dir() + "/Cool/res/shaders/fullscreen.vert",
              fragment_shader_path}
{
    GLDebug(glGenVertexArrays(1, &_dummy_vao_id));
}

FullscreenPipeline::~FullscreenPipeline()
{
    GLDebug(glDeleteVertexArrays(1, &_dummy_vao_id));
}

void FullscreenPipeline::recompile_shader_from_file(std::string_view fragment_shader_path)
{
    _shader = Shader{File::root_dir() + "/Cool/res/shaders/fullscreen.vert",
                     fragment_shader_path};
}

void FullscreenPipeline::recompile_shader_from_source(std::string_view fragment_source_code)
{
    _shader = Shader{{ShaderSource{File::to_string(File::root_dir() + "/Cool/res/shaders/fullscreen.vert"), ShaderKind::Vertex},
                      ShaderSource{fragment_source_code, ShaderKind::Fragment}}};
}

void FullscreenPipeline::draw()
{
    // We use a smart trick to render fullscreen, as explained here : https://stackoverflow.com/a/59739538
    _shader.bind();
    GLDebug(glBindVertexArray(_dummy_vao_id));
    GLDebug(glDrawArrays(GL_TRIANGLES, 0, 3));
}

} // namespace Cool::OpenGL

#endif