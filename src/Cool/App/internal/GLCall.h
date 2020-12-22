#pragma once

#ifndef NDEBUG

namespace Cool {
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