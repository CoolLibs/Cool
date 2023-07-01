#pragma once
#include <map>
#include "Cool/Gpu/TextureSampler.h"
#include "glpp/UniqueHandles/UniqueTextureSampler.h"

namespace Cool {

class TextureSamplerLibrary {
public:
    [[nodiscard]] auto get(TextureSampler const&) -> glpp::UniqueTextureSampler const&;

    void clear() { _samplers.clear(); }

    [[nodiscard]] static auto instance() -> TextureSamplerLibrary&
    {
        static auto inst = TextureSamplerLibrary{};
        return inst;
    }

private:
    std::map<TextureSampler, glpp::UniqueTextureSampler> _samplers{};
};

} // namespace Cool