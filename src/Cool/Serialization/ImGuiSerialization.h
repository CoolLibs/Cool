#pragma once

namespace cereal {

template<class Archive>
void serialize(Archive& archive, ImVec2& vec)
{
    archive(
        cereal::make_nvp("x", vec.x),
        cereal::make_nvp("y", vec.y)
    );
}

template<class Archive>
void serialize(Archive& archive, ImVec4& vec)
{
    archive(
        cereal::make_nvp("x", vec.x),
        cereal::make_nvp("y", vec.y),
        cereal::make_nvp("z", vec.z),
        cereal::make_nvp("w", vec.w)
    );
}

} // namespace cereal