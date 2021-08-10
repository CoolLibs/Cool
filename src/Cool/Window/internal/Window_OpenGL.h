#pragma once
#if defined(__COOL_APP_OPENGL)

namespace Cool {

class Window_OpenGL {
public:
    void        setSwapInterval(int value);
    inline void makeCurrent() { glfwMakeContextCurrent(m_window); }

    void        enableVSync();
    void        disableVSync();
    inline bool isVSyncEnabled() const { return m_bIsVSyncEnabled; }

private:
    bool m_bIsVSyncEnabled = true;
};

} // namespace Cool

#endif