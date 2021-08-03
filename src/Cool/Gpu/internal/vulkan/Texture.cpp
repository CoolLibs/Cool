#include "../../Texture.h"

namespace Cool {

Texture::Texture(std::string_view path)
// : _vku{
//       Cool::Vulkan::context().device(),
//       Cool::Vulkan::context().memprops(),
//   }
{
}

Texture::Id Texture::id()
{
    return {0};
}

} // namespace Cool