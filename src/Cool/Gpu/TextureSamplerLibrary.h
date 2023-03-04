#pragma once
#include <map>
#include "Cool/Gpu/TextureSamplerInfo.h"
#include "glpp/UniqueHandles/UniqueTextureSampler.h"

namespace Cool {

class TextureSamplerLibrary {
public:
    [[nodiscard]] auto get(TextureSamplerInfo const&) -> glpp::UniqueTextureSampler const&;

    void clear() { _samplers.clear(); }

    [[nodiscard]] static auto instance() -> TextureSamplerLibrary&
    {
        static auto inst = TextureSamplerLibrary{};
        return inst;
    }

private:
    std::map<TextureSamplerInfo, glpp::UniqueTextureSampler> _samplers{};
};

} // namespace Cool