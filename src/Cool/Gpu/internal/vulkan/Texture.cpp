#include "../../Texture.h"
#include <Cool/Vulkan/Context.h>

namespace Cool {

Texture::Texture(const ImageData& image_data)
    : _vku{
          Cool::Vulkan::context().g_Device,
          Cool::Vulkan::context().memory_properties,
          image_data.width,
          image_data.height}
{
}

Texture::Texture(std::string_view path)
    : Texture{ImageData{path}}
{
}

Texture::Id Texture::id()
{
    return {0};
}

} // namespace Cool