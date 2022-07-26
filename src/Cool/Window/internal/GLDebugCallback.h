#pragma once

#if defined(COOL_OPENGL) && defined(DEBUG)

#include <Cool/Log/ToUser.h>

namespace Cool {

static std::vector<unsigned int> AlreadydisplayedIds;

void APIENTRY GLDebugCallback(
    GLenum       source,
    GLenum       type,
    unsigned int id,
    GLenum       severity,
    GLsizei /*length*/,
    const char* openGLMessage,
    const void* /*userParam*/
)
{
    // Check if we have already seen the message
    if (std::find(AlreadydisplayedIds.begin(), AlreadydisplayedIds.end(), id) != AlreadydisplayedIds.end())
    {
        return;
    }
    AlreadydisplayedIds.push_back(id);

    // Make message
    std::string message;
    message += "OpenGL Debug message (id=" + std::to_string(id) + ")\n" + openGLMessage;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API: message += "\n\nSource: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: message += "\n\nSource: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: message += "\n\nSource: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: message += "\n\nSource: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION: message += "\n\nSource: Application"; break;
    case GL_DEBUG_SOURCE_OTHER: message += "\n\nSource: Other"; break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: message += "\nType: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: message += "\nType: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: message += "\nType: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY: message += "\nType: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: message += "\nType: Performance"; break;
    case GL_DEBUG_TYPE_MARKER: message += "\nType: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP: message += "\nType: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP: message += "\nType: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER: message += "\nType: Other"; break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: message += "\nSeverity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: message += "\nSeverity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW: message += "\nSeverity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: message += "\nSeverity: notification"; break;
    }

    // Log
    if (type == GL_DEBUG_TYPE_ERROR || type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR || severity == GL_DEBUG_SEVERITY_HIGH)
    {
        Log::Debug::error("OpenGL", message);
    }
    else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || type == GL_DEBUG_TYPE_PORTABILITY)
    {
        Log::Debug::warning("OpenGL", message);
    }
    else
    {
        // Log::info(message);
    }
}

} // namespace Cool

#endif