#pragma once

#include <Cool/History/History.h>

namespace Cool {

/**
 * @brief History where all the changes made to any parameter are stored
 * 
 */
class ParametersHistory {
public:
    inline static History& get() { return _history; }

private:
    static History _history;
};

} // namespace Cool