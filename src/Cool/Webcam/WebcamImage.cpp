#include "WebcamImage.hpp"
#include "Cool/Gpu/Texture.h"

namespace Cool {

// class TexturePool { // TODO not needed, Image can create their own texture (deferred, on the main thread)
// public:
//     TexturePool()
//     {
//         _ids.resize(20);
//         for (auto& id : _ids)
//         {
//             glGenTextures(1, &id);
//             glBindTexture(GL_TEXTURE_2D, id);

//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         }
//     }

//     auto take() -> GLuint
//     {
//         std::scoped_lock lock{_mutex};

//         if (_ids.empty())
//         {
//             assert(false);
//             return 0;
//         }
//         auto const res = _ids.back();
//         _ids.pop_back();
//         return res;
//     }

//     void give_back(GLuint id)
//     {
//         std::scoped_lock lock{_mutex};
//         _ids.push_back(id);
//     }

//     // ~TexturePool{
//     // TODO
//     // glDeleteTextures(1, &_texture_id);
//     // }

// private:
//     std::vector<GLuint> _ids{};
//     std::mutex          _mutex{};
// };

// auto texture_pool() -> TexturePool&
// {
//     static auto instance = TexturePool{};
//     return instance;
// }

// Image()
//     : _texture_id{texture_pool().take()}
// {
// }

// ~Image() override
// {
//     texture_pool().give_back(_texture_id);
// }

auto WebcamImage::get_texture() const -> Texture const&
{
    if (_gen_texture.has_value())
    {
        (*_gen_texture)();
        _gen_texture.reset();
    }
    return *_texture;
}

void WebcamImage::set_data(wcam::ImageDataView<wcam::RGB24> rgb_data)
{
    _gen_texture = [owned_rgb_data = rgb_data.copy(), this]() {
        auto const size = img::Size{
            owned_rgb_data.resolution().width(),
            owned_rgb_data.resolution().height()
        };
        auto const layout = glpp::TextureLayout{
            .internal_format = glpp::InternalFormat::RGBA,
            .channels        = glpp::Channels::RGB,
            .texel_data_type = glpp::TexelDataType::UnsignedByte,
        };
        auto const* data = owned_rgb_data.data();

        if (!_texture)
        {
            _texture = Texture{size, data, layout};
        }
        else
        {
            _texture->set_image(size, data, layout);
        }
    };
}

void WebcamImage::set_data(wcam::ImageDataView<wcam::BGR24> bgr_data)
{
    _gen_texture = [owned_bgr_data = bgr_data.copy(), this]() {
        auto const size = img::Size{
            owned_bgr_data.resolution().width(),
            owned_bgr_data.resolution().height()
        };
        auto const layout = glpp::TextureLayout{
            .internal_format = glpp::InternalFormat::RGBA,
            .channels        = glpp::Channels::BGR,
            .texel_data_type = glpp::TexelDataType::UnsignedByte,
        };
        auto const* data = owned_bgr_data.data();

        if (!_texture)
        {
            _texture = Texture{size, data, layout};
        }
        else
        {
            _texture->set_image(size, data, layout);
        }
    };
}

} // namespace Cool