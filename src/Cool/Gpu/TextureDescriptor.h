#pragma once
#include <compare>
#include "Cool/Gpu/TextureSource_FromWebcam.h"
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

struct TextureDescriptor_FromFile : public TextureDescriptor {
    TextureDescriptor_FromFile()
        : TextureDescriptor{TextureSource_FromFile{}, {}}
    {}
};
struct TextureDescriptor_FromWebcam : public TextureDescriptor {
    TextureDescriptor_FromWebcam()
        : TextureDescriptor{TextureSource_FromWebcam{}, {}}
    {}
};

} // namespace Cool