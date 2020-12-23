#pragma once

#include "Pixel.h"

namespace Cool {

class Image;

class ImageIterator {
public:
	ImageIterator(Image& image, unsigned int x, unsigned int y);
	~ImageIterator() = default;

	Pixel operator*();
	void operator++();
	bool operator!=(ImageIterator& it);

private:
	Image& m_image;
	unsigned int m_x;
	unsigned int m_y;
};

} // namespace Cool