#pragma once

namespace cereal {

template<class Archive>
void serialize(Archive& archive, ImVec4& vec)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(
        cereal::make_nvp("x", vec.x),
        cereal::make_nvp("y", vec.y),
        cereal::make_nvp("z", vec.z),
        cereal::make_nvp("w", vec.w)
    );
    // #else
    //     (void)archive;
    // #endif
}

} // namespace cereal