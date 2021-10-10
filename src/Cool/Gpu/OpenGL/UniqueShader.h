#pragma once
#if defined(COOL_OPENGL)

namespace Cool {

class UniqueShader {
public:
    UniqueShader();
    ~UniqueShader();
    UniqueShader(const UniqueShader&) = delete;
    UniqueShader& operator=(const UniqueShader&) = delete;
    UniqueShader(UniqueShader&& rhs) noexcept;
    UniqueShader& operator=(UniqueShader&& rhs) noexcept;

    auto id() const { return _id; }

private:
    GLuint _id;
};

} // namespace Cool

#endif