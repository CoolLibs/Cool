#include "TextureSamplerLibrary.hpp"

namespace Cool {

auto TextureSamplerLibrary::get(TextureSamplerDescriptor const& desc) -> TextureSampler const&
{
    // Try to find an existing sampler with the same desc
    auto const it = _samplers.find(desc);
    if (it != _samplers.end())
        return it->second;

    // Create a new sampler
    return _samplers.insert(std::make_pair(desc, TextureSampler{desc})).first->second;
}

} // namespace Cool