#pragma once

namespace Cool {

struct Image {
    std::vector<unsigned char> data;
    int                        width;
    int                        height;
};
} // namespace Cool