#if defined(COOL_OPENGL)

#include "FullscreenPipeline.h"
#include <Cool/File/File.h>
#include "try_make_shader.h"

namespace Cool::OpenGL {

FullscreenPipeline::FullscreenPipeline(std::string_view fragment_shader_source_code)
{
    compile(fragment_shader_source_code);
}

void FullscreenPipeline::compile(std::string_view fragment_shader_source_code)
{
    _shader = try_make_shader({ShaderDescription{File::to_string(File::root_dir() + "/Cool/res/shaders/fullscreen.vert"), ShaderKind::Vertex},
                               ShaderDescription{std::string{fragment_shader_source_code}, ShaderKind::Fragment}});
}

void FullscreenPipeline::draw()
{
    if (_shader.has_value()) {
        // We use a smart trick to render fullscreen, as explained here : https://stackoverflow.com/a/59739538
        _shader->bind();
        GLDebug(glBindVertexArray(_dummy_vao.id()));
        GLDebug(glDrawArrays(GL_TRIANGLES, 0, 3));
    }
}

} // namespace Cool::OpenGL

#endif