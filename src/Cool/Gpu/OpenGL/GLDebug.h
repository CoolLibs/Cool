#pragma once

#ifdef COOL_OPENGL
#ifdef DEBUG

namespace Cool {
/// Wrap all you OpenGL calls in this macro : it will add debug checks. Something like : GLDebug(GLuint programID = glCreateProgram())
/// It is not strictly necessary though because modern OpenGL debugging is enabled too. It's only to help those who don't have the advandced debugging available on their machine.
#define GLDebug(x)                          \
    CoolGlDebug::clearFromPreviousErrors(); \
    x;                                      \
    assert(!CoolGlDebug::checkForErrors(#x, __FILE__, __LINE__))
} // namespace Cool

namespace CoolGlDebug {
void        clearFromPreviousErrors();
bool        checkForErrors(const char* functionName, const char* filename, int line);
char const* glErrorString(GLenum const err);
} // namespace CoolGlDebug

#else
namespace Cool {
#define GLDebug(x) x
} // namespace Cool
#endif
#endif