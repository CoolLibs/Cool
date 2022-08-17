namespace Cool::OpenGL {

namespace internal {

template<typename... ShaderModules>
void attach_modules(GLuint id, ShaderModules&&... shader_modules)
{
    (glAttachShader(id, shader_modules.id()), ...);
}

template<typename... ShaderModules>
void detach_modules(GLuint id, ShaderModules&&... shader_modules)
{
    (glDetachShader(id, shader_modules.id()), ...);
}

inline void check_for_linking_errors(GLuint id)
{
    int result; // NOLINT
    GLDebug(glGetProgramiv(id, GL_LINK_STATUS, &result));
    if (result == GL_FALSE)
    {
        GLsizei length; // NOLINT
        GLDebug(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> error_message;
        error_message.reserve(static_cast<size_t>(length));
        GLDebug(glGetProgramInfoLog(id, length, nullptr, error_message.data()));
        throw std::invalid_argument(std::string{"Linking failed:\n"} + error_message.data());
    }
}

} // namespace internal

template<typename... ShaderModules>
Shader::Shader(ShaderModules&&... shader_modules)
{
    internal::attach_modules(_shader.id(), std::forward<ShaderModules>(shader_modules)...);
    GLDebug(glLinkProgram(_shader.id()));
    internal::detach_modules(_shader.id(), std::forward<ShaderModules>(shader_modules)...);
    internal::check_for_linking_errors(_shader.id());
}

} // namespace Cool::OpenGL