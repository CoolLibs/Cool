#pragma once

namespace Cool {

class Input {
public:
	/// <summary>
	/// Checks if a glfw key code corresponds to a given character in the current keyboard layout
	/// </summary>
	/// <param name="character">A const char* containing the single character that you want to check against</param>
	/// <param name="key">The key code that you want to check</param>
	static bool MatchesChar(const char* character, int key);
	static bool KeyIsDown(SDL_Scancode key);
	/// Relative to the viewing area, not the whole window !
	/// (0, 0) is in the top-left corner
	static glm::ivec2 MouseInPixels();
	/// Relative to the viewing area, not the whole window !
	/// (0, 0) is in the top-left corner
	static glm::vec2 MouseInCentimeters();
	/// Relative to the viewing area, not the whole window !
	/// (0, 0) is in the middle
	/// x is in the range [-aspectRatio, +aspectRatio] Axis pointing right
	/// y is in the range [-1, 1] Axis poiting up
	static glm::vec2  MouseInNormalizedRatioSpace();

private:
friend class AppManager;
	static void Initialize();

private:
	/// Multiply a vec2 expressed in pixels by this to get a vec2 expressed in centimeters
	static glm::vec2 s_PixelsToCentimeters;
#ifndef NDEBUG
	static bool s_initialized;
#endif
};

} // namespace Cool