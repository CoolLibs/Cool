#pragma once

#include "Color.h"

namespace Cool {

class Image;

/// The position and color of a given pixel of a given image.
/// No need to take this class by reference : it already holds a reference to the image and can modify it.
class Pixel {
public:
	Pixel(Image& image, unsigned int x, unsigned int y);
	~Pixel() = default;
	
	/// A reference to the image's color at this pixel
	Color& color();
	/// <summary>
	/// In the range [-aspectRatio, +aspectRatio]
	/// Axis pointing right
	/// </summary>
	float x();
	/// <summary>
	/// In the range [-1, 1]
	/// Axis pointing up
	/// </summary>
	float y();
	/// <summary>
	/// Position expressed in number of pixels, ranging from 0 to width-1
	/// </summary>
	inline int xInt() { return m_x; }
	/// <summary>
	/// Position expressed in number of pixels, ranging from 0 to height-1
	/// </summary>
	inline int yInt() { return m_y; }

private:
	Image& m_image;
	unsigned int m_x;
	unsigned int m_y;
};

} // namespace Cool