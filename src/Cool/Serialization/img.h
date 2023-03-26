#pragma once

#include <img/img.hpp>

namespace cereal {

template<class Archive, typename T>
void save(Archive& archive, img::SizeT<T> size)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    archive(
        cereal::make_nvp("w", size.width()),
        cereal::make_nvp("h", size.height())
    );
    // #else
    //     (void)archive;
    //     (void)size;
    // #endif
}

template<class Archive, typename T>
void load(Archive& archive, img::SizeT<T>& size)
{
    // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
    T tmpW;
    T tmpH;
    archive(tmpW, tmpH);
    size.set_width(tmpW);
    size.set_height(tmpH);
    // #else
    //         (void)archive;
    //         (void)size;
    // #endif
}

} // namespace cereal