#pragma once

#include <img/img.hpp>

namespace cereal {

template<class Archive, typename T>
void save(Archive& archive, img::SizeT<T> size)
{
    archive(
        cereal::make_nvp("w", size.width()),
        cereal::make_nvp("h", size.height())
    );
}

template<class Archive, typename T>
void load(Archive& archive, img::SizeT<T>& size)
{
    T tmpW;
    T tmpH;
    archive(tmpW, tmpH);
    size.set_width(tmpW);
    size.set_height(tmpH);
}

} // namespace cereal