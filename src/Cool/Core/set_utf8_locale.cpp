#include "set_utf8_locale.hpp"

namespace Cool {

void set_utf8_locale()
{
    if (!std::setlocale(LC_ALL,
#ifdef _WIN32
                        ".65001" // utf-8
#else
                        "UTF-8"
#endif
        ))
    {
        assert(false);
    }
}

} // namespace Cool