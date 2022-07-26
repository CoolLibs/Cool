#if defined(COOL_OPENGL) && defined(DEBUG)

#include "GLDebug.h"

void CoolGlDebug::clearFromPreviousErrors()
{
    while (glGetError() != GL_NO_ERROR)
    {
    }
}

bool CoolGlDebug::checkForErrors(const char* functionName, const char* filename, int line)
{
    GLenum error; // NOLINT
    bool   bFoundErrors = false;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        Cool::Log::error_without_breakpoint(
            "OpenGL",
            std::string{glErrorString(error)} + " : " + std::string{functionName} + " " + std::string{filename} + " " + std::to_string(line)
        );
        bFoundErrors = true;
    }
    return bFoundErrors;
}

char const* CoolGlDebug::glErrorString(GLenum const err)
{
    switch (err)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";

    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";

    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";

    default:
        return "UNKNOWN_ERROR_TYPE";
    }
}

#endif