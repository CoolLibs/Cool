#pragma once

namespace Cool {

class Uuid {
public:
    Uuid();
    explicit Uuid(int64_t uuid)
        : _uuid{uuid}
    {
    }
    operator int64_t() const { return _uuid; }

private:
    int64_t _uuid;
};

} // namespace Cool
