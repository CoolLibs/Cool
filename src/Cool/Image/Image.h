#pragma once

#include "internal/ImageIterator.h"
#include "internal/Color.h"

namespace Cool {

/**
 * @brief Wrapper for a vector of Colors (4 unsigned chars) representing an image
 * 
 */
class Image {
public:
	/**
	 * @brief Creates an image with each pixel set as (0, 0, 0, 255) (black with 100% opacity)
	 * 
	 * @param width 
	 * @param height 
	 */
	Image(unsigned int width, unsigned int height);

	/**
	 * @brief Saves the image as a PNG. If the folders present in the path don't exist, they will be created.
	 * 
	 * @param filepath path to save the image to. Something like "out/myImage.png"
	 */
	void save_as(const char* filepath);

	/**
	 * @brief 
	 * 
	 * @return The width in pixels
	 */
	inline unsigned int width()  { return _width; }
	
	/**
	 * @brief 
	 * 
	 * @return The height in pixels
	 */
	inline unsigned int height() { return _height; }

	/**
	 * @brief 
	 * 
	 * @return Width divided by height 
	 */
	inline float aspect_ratio() { return _aspect_ratio; }


	/**
	 * @brief 
	 * 
	 * @return The full array of unsigned chars representing the image, with four channels per pixel. Follows the OpenGL convention : the first pixel in the array is the bottom left of the image, the second pixel is on the first row second column etc.
	 */
	inline unsigned char* data() { return (unsigned char*)_pixel_colors.data(); }

	/**
	 * @brief Returns the Color of the (x, y) pixel. No bound checking is done, so this will crash if x is not inside [0, width() - 1] or y is not inside [0, height() - 1]
	 * 
	 * @param x x coordinate of the pixel (0 is at the left of the image)
	 * @param y y coordinate of the pixel (0 is at the bottom of the image)
	 * @return The Color of the given pixel
	 */
	inline Color& color_at(unsigned int x, unsigned int y) { return _pixel_colors[x + y * _width]; }

	ImageIterator begin();
	ImageIterator end();

private:
	unsigned int _width;
	unsigned int _height;
	float _aspect_ratio;
	/**
	 * @brief Follows OpenGL's convention : (0, 0) is the bottom-left of the image. Pixels are ordered by row first (As you read through the vector x increases up to width-1 (included), then wraps back to 0 and y goes up by 1, and so on)
	 * 
	 */
	std::vector<Color> _pixel_colors;
};

} // namespace Cool