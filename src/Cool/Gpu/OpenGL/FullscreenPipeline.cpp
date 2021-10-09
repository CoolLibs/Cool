#if defined(COOL_OPENGL)

#include "FullscreenPipeline.h"
#include <Cool/File/File.h>
#include "try_make_shader.h"

namespace Cool::OpenGL {

static std::vector<ShaderDescription> shader_descriptions(std::string_view source)
{
    return {ShaderDescription{File::to_string(File::root_dir() + "/Cool/res/shaders/fullscreen.vert"), ShaderKind::Vertex},
            ShaderDescription{std::string{source}, ShaderKind::Fragment}};
}

FullscreenPipeline::FullscreenPipeline(std::string_view source)
    : _shader{try_make_shader(shader_descriptions(source))}
{
    GLDebug(glGenVertexArrays(1, &_dummy_vao_id));
}

FullscreenPipeline::~FullscreenPipeline()
{
    GLDebug(glDeleteVertexArrays(1, &_dummy_vao_id));
}

void FullscreenPipeline::recompile(std::string_view source)
{
    _shader = try_make_shader(shader_descriptions(source));
}

void FullscreenPipeline::draw()
{
    if (_shader.has_value()) {
        // We use a smart trick to render fullscreen, as explained here : https://stackoverflow.com/a/59739538
        _shader->bind();
        GLDebug(glBindVertexArray(_dummy_vao_id));
        GLDebug(glDrawArrays(GL_TRIANGLES, 0, 3));
    }
}

} // namespace Cool::OpenGL

#endif