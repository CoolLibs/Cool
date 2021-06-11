#pragma once

#include "Color.h"

namespace Cool {

class Image;

/**
 * @brief The position and color of a given pixel of a given image. No need to take this class by reference : it already holds a reference to the image and can modify it.
 * 
 */
class Pixel {
public:
	Pixel(Image& image, unsigned int x, unsigned int y);
	
	/**
	 * @brief 
	 * 
	 * @return A reference to the image's color at this pixel
	 */
	Color& color();

	/**
	 * @brief 
	 * 
	 * @return x coordinate of the pixel. In the range [-aspect_ratio, +aspect_ratio]. Axis pointing right 
	 */
	float x();

	/**
	 * @brief 
	 * 
	 * @return y coordinate of the pixel. In the range [-1, +1]. Axis pointing up 
	 */
	float y();

	/**
	 * @brief 
	 * 
	 * @return Position expressed in number of pixels, ranging from 0 to width-1
	 */
	inline int x_as_int() { return _x; }
	
	/**
	 * @brief 
	 * 
	 * @return Position expressed in number of pixels, ranging from 0 to height-1 
	 */
	inline int y_as_int() { return _y; }

private:
	Image& _image;
	unsigned int _x;
	unsigned int _y;
};

} // namespace Cool