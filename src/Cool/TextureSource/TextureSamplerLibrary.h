#pragma once
#include <map>
#include "Cool/TextureSource/TextureSamplerDescriptor.h"

namespace Cool {

class TextureSamplerLibrary {
public:
    // [[nodiscard]] auto get(TextureSamplerDescriptor const&) -> glpp::UniqueTextureSampler const&;

    [[nodiscard]] static auto instance() -> TextureSamplerLibrary&
    {
        static auto inst = TextureSamplerLibrary{};
        return inst;
    }

private:
    // std::map<TextureSamplerDescriptor, glpp::UniqueTextureSampler> _samplers{};
};

} // namespace Cool