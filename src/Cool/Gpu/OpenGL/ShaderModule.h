#pragma once

#include "../ShaderDescription.h"

namespace Cool::OpenGL {

class ShaderModule {
public:
    /**
     * @brief Throws if there is an error while compiling the shader source code
     */
    explicit ShaderModule(const ShaderDescription& desc);
    ~ShaderModule();
    ShaderModule(const ShaderModule&) = delete;
    ShaderModule& operator=(const ShaderModule&) = delete;
    ShaderModule(ShaderModule&& rhs) noexcept;
    ShaderModule& operator=(ShaderModule&& rhs) noexcept;

    GLuint id() const { return _id; }

private:
    GLuint _id;
};

} // namespace Cool::OpenGL
