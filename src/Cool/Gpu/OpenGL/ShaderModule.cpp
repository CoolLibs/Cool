#include "ShaderModule.h"

namespace Cool::OpenGL {

ShaderModule::ShaderModule(const ShaderDescription& desc)
{
    const GLenum shader_type = [&]() {
        switch (desc.kind) {
        case ShaderKind::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderKind::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderKind::Geometry:
            return GL_GEOMETRY_SHADER;
        case ShaderKind::Compute:
            return GL_COMPUTE_SHADER;
        default:
            Log::error("Unknown shader type !");
            return 0;
        }
    }();
    // Create
    GLDebug(_id = glCreateShader(shader_type));
    // Compile
    const char* src = desc.source_code.c_str();
    GLDebug(glShaderSource(_id, 1, &src, nullptr));
    GLDebug(glCompileShader(_id));
// Debug
#if defined(DEBUG)
    int result; // NOLINT
    GLDebug(glGetShaderiv(_id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length; // NOLINT
        GLDebug(glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char)); // NOLINT
        GLDebug(glGetShaderInfoLog(_id, length, &length, message));
        Log::error("Shader compilation failed :\n{}", message);
        GLDebug(glDeleteShader(_id));
    }
#endif
}

ShaderModule::~ShaderModule()
{
    GLDebug(glDeleteShader(_id));
}

} // namespace Cool::OpenGL
