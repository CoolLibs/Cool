#pragma once

namespace Cool::Math {
	/**
	 * @brief Returns +1 or -1 depending on the sign of val
	 * 
	 * @tparam T Any number type
	 * @param val 
	 * @return  
	 */
	template <typename T> int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}
}