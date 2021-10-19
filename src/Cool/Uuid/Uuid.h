#pragma once

namespace Cool {

class Uuid {
public:
    Uuid();
    operator int64_t() const { return _uuid; }

private:
    int64_t _uuid;
};

} // namespace Cool
