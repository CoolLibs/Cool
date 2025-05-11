#pragma once
#include <compare>
#include "Cool/Path/Path.h"
#include "Cool/WebGPU/Texture.h"
#include "Cool/WebGPU/TextureSamplerDescriptor.hpp"
#include "TextureSource.h"

namespace Cool {

// TODO(WebGPU) Rename, as this can cause confusion with wgpu::TextureDescriptor
struct TextureDescriptor {
    TextureSource            source;
    TextureSamplerDescriptor sampler{};

    auto imgui_widget() -> bool;

    friend auto operator==(TextureDescriptor const& a, TextureDescriptor const& b) -> bool = default;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Source", source),
            ser20::make_nvp("Sampler", sampler)
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