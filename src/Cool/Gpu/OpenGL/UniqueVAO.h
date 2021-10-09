#pragma once
#if defined(COOL_OPENGL)

namespace Cool {

class UniqueVAO {
public:
    UniqueVAO();
    ~UniqueVAO();
    UniqueVAO(const UniqueVAO&) = delete;
    UniqueVAO& operator=(const UniqueVAO&) = delete;
    UniqueVAO(UniqueVAO&& rhs) noexcept;
    UniqueVAO& operator=(UniqueVAO&& rhs) noexcept;

    auto id() const -> GLuint { return _id; }

private:
    GLuint _id;
};

} // namespace Cool

#endif