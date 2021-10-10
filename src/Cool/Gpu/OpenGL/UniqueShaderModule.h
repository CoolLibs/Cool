#pragma once

#include "../ShaderKind.h"

namespace Cool {

class UniqueShaderModule {
public:
    explicit UniqueShaderModule(ShaderKind shader_kind);
    ~UniqueShaderModule();
    UniqueShaderModule(const UniqueShaderModule&) = delete;
    UniqueShaderModule& operator=(const UniqueShaderModule&) = delete;
    UniqueShaderModule(UniqueShaderModule&& rhs) noexcept;
    UniqueShaderModule& operator=(UniqueShaderModule&& rhs) noexcept;

    auto id() const { return _id; }

private:
    GLuint _id;
};

} // namespace Cool
