#include "ImageIterator.h"
#include "../Image.h"

namespace Cool {

ImageIterator::ImageIterator(Image& image, unsigned int x, unsigned int y)
	: _image(image), _x(x), _y(y)
{}

Pixel ImageIterator::operator*() {
	return Pixel(_image, _x, _y);
}

void ImageIterator::operator++() {
	_x++;
	if (_x == _image.width()) {
		_x = 0;
		_y++;
	}
}

bool ImageIterator::operator!=(ImageIterator& it) {
	return   _x != it._x 
		|| _y != it._y;
}

} // namespace Cool