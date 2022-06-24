#pragma once

namespace Cool {

/**
 * @brief Color stored as 4 unsigned chars representing Red, Green, Blue and Alpha channels
 *
 */
class Color {
public:
    /**
     * @brief Initialized as black with 255 alpha : (0, 0, 0, 255)
     *
     */
    Color();

    /**
     * @brief Takes values in the range [0, 255] (values outside will wrap)
     *
     * @param r
     * @param g
     * @param b
     */
    void set(unsigned char r, unsigned char g, unsigned char b);

    /**
     * @brief Takes values in the range [0, 1] (values outside are clamped to 0 or 1)
     *
     * @param r
     * @param g
     * @param b
     */
    void set(float r, float g, float b);

    /**
     * @brief Takes values in the range [0, 1] (values outside are clamped to 0 or 1)
     *
     * @param color
     */
    inline void set(const glm::vec3& color) { set(color.r, color.g, color.b); }

private:
    unsigned char _channels[4];
};

} // namespace Cool