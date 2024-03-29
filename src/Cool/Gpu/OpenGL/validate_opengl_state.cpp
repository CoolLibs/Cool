#if COOL_OPENGL
#include "validate_opengl_state.h"

/// This function can be called just before a draw call to make sure everything is set up correctly.
void validate_opengl_state(GLuint id)
{
    GLint result;
    GLDebug(glGetProgramiv(id, GL_VALIDATE_STATUS, &result));
    if (result == GL_FALSE)
    {
        GLsizei length;
        GLDebug(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));
        if (length > 0)
        {
            std::vector<GLchar> error_message;
            error_message.resize(static_cast<size_t>(length));
            GLDebug(glGetProgramInfoLog(id, length, nullptr, error_message.data()));
            throw std::invalid_argument(std::string{"Validation failed:\n"} + error_message.data());
        }
        else
        {
            throw std::invalid_argument("Validation failed:\nNo error message available.");
        }
    }
}

#endif
