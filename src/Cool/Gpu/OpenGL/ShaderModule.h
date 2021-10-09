#pragma once

#include "../ShaderDescription.h"

namespace Cool::OpenGL {

class ShaderModule {
public:
    explicit ShaderModule(const ShaderDescription& desc);
    ~ShaderModule();

    GLuint id() const { return _id; }

private:
    GLuint _id;
};

} // namespace Cool::OpenGL
