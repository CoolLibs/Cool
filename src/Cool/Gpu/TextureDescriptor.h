#pragma once
#include <compare>
#include "Cool/Gpu/Texture.h"
#include "Cool/Path/Path.h"
#include "TextureSamplerDescriptor.h"
#include "TextureSource.h"

namespace Cool {

struct TextureDescriptor {
    TextureSource            source;
    TextureSamplerDescriptor sampler{};

    auto imgui_widget() -> bool;

    friend auto operator==(TextureDescriptor const& a, TextureDescriptor const& b) -> bool = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Source", source),
            cereal::make_nvp("Sampler", sampler)
        );
    }
};

struct TextureDescriptor_Image : public TextureDescriptor {
    TextureDescriptor_Image()
        : TextureDescriptor{TextureSource_Image{}, {}}
    {}
};
struct TextureDescriptor_Webcam : public TextureDescriptor {
    TextureDescriptor_Webcam()
        : TextureDescriptor{TextureSource_Webcam{}, {}}
    {}
};
struct TextureDescriptor_Video : public TextureDescriptor {
    TextureDescriptor_Video()
        : TextureDescriptor{TextureSource_Video{}, {}}
    {}
};

} // namespace Cool