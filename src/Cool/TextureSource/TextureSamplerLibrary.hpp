#pragma once
#include <map>
#include "Cool/WebGPU/TextureSampler.hpp"
#include "Cool/WebGPU/TextureSamplerDescriptor.hpp"

namespace Cool {

class TextureSamplerLibrary {
public:
    [[nodiscard]] auto get(TextureSamplerDescriptor const&) -> TextureSampler const&;

private:
    std::map<TextureSamplerDescriptor, TextureSampler> _samplers{};
};

[[nodiscard]] inline auto texture_sampler_library() -> TextureSamplerLibrary&
{
    static auto inst = TextureSamplerLibrary{};
    return inst;
}

} // namespace Cool