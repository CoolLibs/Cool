namespace Cool::OpenGL {

namespace details {

template<typename... ShaderModules>
void attach_modules(GLuint id, ShaderModules&&... shader_modules)
{
    (glAttachShader(id, shader_modules.id()), ...);
}

static void validate_shader(GLuint id)
{
    GLDebug(glValidateProgram(id));
    GLint result;
    GLDebug(glGetProgramiv(id, GL_VALIDATE_STATUS, &result));
    if (result == GL_FALSE) {
        GLsizei length;
        GLDebug(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));
        if (length > 0) {
            std::vector<GLchar> error_message;
            error_message.reserve(length);
            GLDebug(glGetProgramInfoLog(id, length, nullptr, error_message.data()));
            throw std::invalid_argument(std::string{"Linking failed:\n"} + error_message.data());
        }
        else {
            throw std::invalid_argument("Linking failed:\nNo error message available.");
        }
    }
}

} // namespace details

template<typename... ShaderModules>
Shader::Shader(ShaderModules&&... shader_modules)
{
    details::attach_modules(_shader.id(), std::forward<ShaderModules>(shader_modules)...);
    GLDebug(glLinkProgram(_shader.id()));
    details::validate_shader(_shader.id());
}

} // namespace Cool::OpenGL