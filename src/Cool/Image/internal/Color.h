#pragma once

namespace Cool {

/// Color stored as 4 unsigned chars representing Red, Green, Blue and Alpha channels
class Color {
public:
	Color();
	~Color() = default;

	/// <summary>
	/// Takes values in the range [0, 255] (values outside will wrap)
	/// </summary>
	void set(unsigned char r, unsigned char g, unsigned char b);
	/// <summary>
	/// Takes values in the range [0, 1] (values outside are clamped to 0 or 1)
	/// </summary>
	void set(float r, float g, float b);
#ifdef __COOL_GLM
	/// <summary>
	/// Takes values in the range [0, 1] (values outside are clamped to 0 or 1)
	/// </summary>
	inline void set(const glm::vec3& color) { set(color.r, color.g, color.b); }
#endif // __COOL_GLM

private:
	unsigned char m_channels[4];
};

}