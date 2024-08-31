#include "TextureRef.hpp"
#include "glpp/glpp.hpp"

namespace Cool {

auto TextureRef::download_pixels() const -> img::Image
{
    glpp::bind_texture<glpp::TextureKind::Tex2D>(id);
    std::unique_ptr<uint8_t[]> data{new uint8_t[4 * width() * height()]};
    glReadPixels(0, 0, static_cast<GLsizei>(width()), static_cast<GLsizei>(height()), GL_RGBA, GL_UNSIGNED_BYTE, data.get());
    glpp::bind_texture<glpp::TextureKind::Tex2D>(0);
    return img::Image{
        img::Size{width(), height()},
        4,
        data.release()
    };
}

} // namespace Cool