#if defined(COOL_OPENGL)

#include "FullscreenPipeline.h"
#include <Cool/File/File.h>
#include "try_make_shader.h"

namespace Cool::OpenGL {

const ShaderModule& vertex_module()
{
    static const ShaderModule shader_module{{File::to_string(File::root_dir() + "/Cool/res/shaders/fullscreen.vert"),
                                             ShaderKind::Vertex,
                                             "Cool's fullscreen vertex shader"}};
    return shader_module;
}

FullscreenPipeline::FullscreenPipeline(std::string_view fragment_shader_source_code, std::string_view name)
{
    compile(fragment_shader_source_code, name);
}

void FullscreenPipeline::compile(std::string_view fragment_shader_source_code, std::string_view name)
{
    try {
        const ShaderModule fragment_module{{std::string{fragment_shader_source_code},
                                            ShaderKind::Fragment,
                                            std::string{name}}};
        _shader = Shader{{&vertex_module(), &fragment_module}};
    }
    catch (const std::exception& e) {
        _shader.reset();
    }
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