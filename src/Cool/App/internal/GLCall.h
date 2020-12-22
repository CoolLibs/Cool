#pragma once

#ifndef NDEBUG

namespace Cool {
    /// Wrap all you OpenGL calls with this macro : it will add debug checks.
    /// It is not strictly necessary though because modern OpenGL debugging is enabled too. It's only to help those who don't have the advandced debugging available on their machine.
    #define GLCall(x) CoolGlDebug::clearFromPreviousErrors(); x; assert(!CoolGlDebug::checkForErrors(#x, __FILE__, __LINE__))
}

namespace CoolGlDebug {
    void clearFromPreviousErrors();
    bool checkForErrors(const char* functionName, const char* filename, int line);
    char const* glErrorString(GLenum const err);
}

#else
namespace Cool {
    #define GLCall(x) x
}
#endif