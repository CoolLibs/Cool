#pragma once

#include "Pixel.h"

namespace Cool {

class Image;

class ImageIterator {
public:
	ImageIterator(Image& image, unsigned int x, unsigned int y);

	Pixel operator*();
	void operator++();
	bool operator!=(ImageIterator& it);

private:
	Image& _image;
	unsigned int _x;
	unsigned int _y;
};

} // namespace Cool