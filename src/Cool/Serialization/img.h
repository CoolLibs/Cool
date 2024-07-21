#pragma once

#include <img/img.hpp>

namespace ser20 {

template<class Archive, typename T>
void save(Archive& archive, img::SizeT<T> size)
{
    archive(
        ser20::make_nvp("w", size.width()),
        ser20::make_nvp("h", size.height())
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

} // namespace ser20