#if defined(COOL_OPENGL)

#include "../FullscreenPipeline.h"
#include <Cool/File/File.h>
#include "../../ShaderDescription.h"

namespace Cool::OpenGL {

FullscreenPipeline::FullscreenPipeline(const ShaderSource& source)
    : _shader{{ShaderDescription{ShaderSource{File::to_string(File::root_dir() + "/Cool/res/shaders/fullscreen.vert")}, ShaderKind::Vertex},
               ShaderDescription{source, ShaderKind::Fragment}}}
{
    GLDebug(glGenVertexArrays(1, &_dummy_vao_id));
}

FullscreenPipeline::~FullscreenPipeline()
{
    GLDebug(glDeleteVertexArrays(1, &_dummy_vao_id));
}

void FullscreenPipeline::recompile(const ShaderSource& source)
{
    _shader = Shader{{{ShaderSource{File::to_string(File::root_dir() + "/Cool/res/shaders/fullscreen.vert")}, ShaderKind::Vertex},
                      {source, ShaderKind::Fragment}}};
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