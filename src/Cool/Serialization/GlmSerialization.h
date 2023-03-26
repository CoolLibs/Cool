#pragma once

namespace cereal {

template<class Archive>
void serialize(Archive& archive, glm::vec2& v)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(
        cereal::make_nvp("x", v.x),
        cereal::make_nvp("y", v.y)
    );
    // #else
    //     (void)archive;
    // #endif
}

template<class Archive>
void serialize(Archive& archive, glm::vec3& v)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(
        cereal::make_nvp("x", v.x),
        cereal::make_nvp("y", v.y),
        cereal::make_nvp("z", v.z)
    );
    // #else
    //     (void)archive;
    // #endif
}

template<class Archive>
void serialize(Archive& archive, glm::vec4& v)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(
        cereal::make_nvp("x", v.x),
        cereal::make_nvp("y", v.y),
        cereal::make_nvp("z", v.z),
        cereal::make_nvp("w", v.w)
    );
    // #else
    //     (void)archive;
    // #endif
}

template<class Archive>
void serialize(Archive& archive, glm::mat2& m)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(
        m[0][0], m[0][1],
        m[1][0], m[1][1]
    );
    // #else
    //     (void)archive;
    // #endif
}

template<class Archive>
void serialize(Archive& archive, glm::mat3& m)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2]
    );
    // #else
    //         (void)archive;
    // #endif
}

template<class Archive>
void serialize(Archive& archive, glm::mat4& m)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3]
    );
    // #else
    //         (void)archive;
    // #endif
}

} // namespace cereal