#pragma once

namespace Cool::Math {
	/// <summary>
	/// Returns +1 or -1 depending on the sign of val
	/// </summary>
	template <typename T> int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}
}