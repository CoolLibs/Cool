#pragma once

#include <Cool/History/History.h>

namespace Cool {

class ParamsHistory {
public:
	inline static History& get() { return _history; }
private:
	static History _history;
};

} // namespace Cool