#pragma once

#include "internal/ImageIterator.h"
#include "internal/Color.h"

namespace Cool {

/// Wrapper for a vector of Colors (4 unsigned chars) representing an image
class Image {
public:
	/// <summary>
	/// Creates an image with each pixel set as (0, 0, 0, 255) (black with 100% opacity)
	/// </summary>
	Image(unsigned int width, unsigned int height);
	~Image() = default;

#if defined(__COOL_EXPORT_IMAGE) && defined(__COOL_FILE)
	/// <summary>
	/// Saves the image as a PNG. If the folders present in the path don't exist, they will be created.
	/// </summary>
	void saveAs(const char* filepath);
#endif // __COOL_EXPORT_IMAGE && __COOL_FILE
	/// <summary>
	/// Returns the width in pixels
	/// </summary>
	inline unsigned int width()  { return m_width; }
	/// <summary>
	/// Returns the height in pixels
	/// </summary>
	inline unsigned int height() { return m_height; }
	/// <summary>
	/// Returns width/height
	/// </summary>
	inline float aspectRatio() { return m_aspectRatio; }
	/// <summary>
	/// Returns the full array of unsigned chars representing the image, with four channels per pixel.
	/// Follows the OpenGL convention : the first pixel in the array is the bottom left of the image, the second pixel is on the first row second column etc.
	/// </summary>
	inline unsigned char* data() { return (unsigned char*)m_pixelColors.data(); }
	/// <summary>
	/// Returns the Color of a given pixel
	/// </summary>
	inline Color& colorAt(unsigned int x, unsigned int y) { return m_pixelColors[x + y * m_width]; }

	ImageIterator begin();
	ImageIterator end();

private:
	unsigned int m_width;
	unsigned int m_height;
	float m_aspectRatio;
	/// Follows OpenGL's convention : (0, 0) is the bottom-left of the image
	/// Pixels are ordered by row first (As you read through the vector x increases up to width-1 (included), then wraps back to 0 and y goes up by 1, and so on)
	std::vector<Color> m_pixelColors;
};

} // namespace Cool