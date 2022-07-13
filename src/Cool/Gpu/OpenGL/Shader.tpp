namespace Cool::OpenGL {

namespace details {

template<typename... ShaderModules>
void attach_modules(GLuint id, ShaderModules&&... shader_modules)
{
    (glAttachShader(id, shader_modules.id()), ...);
}

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function" // Dunno why clang can't see that it is used
#endif
inline void validate_shader(GLuint id)
{
    GLDebug(glValidateProgram(id));
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

} // namespace details

template<typename... ShaderModules>
Shader::Shader(ShaderModules&&... shader_modules)
{
    details::attach_modules(_shader.id(), std::forward<ShaderModules>(shader_modules)...);
    GLDebug(glLinkProgram(_shader.id()));
    details::validate_shader(_shader.id());
}

} // namespace Cool::OpenGL