#if defined(COOL_OPENGL)

#include "FullscreenPipeline.h"
#include <Cool/File/File.h>
#include "try_make_shader.h"

namespace Cool::OpenGL {

FullscreenPipeline::FullscreenPipeline(std::string_view fragment_shader_source_code, std::string_view name)
{
    compile(fragment_shader_source_code, name);
}

void FullscreenPipeline::compile(std::string_view fragment_shader_source_code, std::string_view name)
{
    _shader = try_make_shader({ShaderDescription{File::to_string(File::root_dir() + "/Cool/res/shaders/fullscreen.vert"), ShaderKind::Vertex, "Cool's fullscreen vertex shader"},
                               ShaderDescription{std::string{fragment_shader_source_code}, ShaderKind::Fragment, std::string{name}}});
}

void FullscreenPipeline::draw() const
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