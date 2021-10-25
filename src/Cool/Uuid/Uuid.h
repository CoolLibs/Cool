#pragma once

namespace Cool {

class Uuid {
public:
    Uuid();
    explicit Uuid(uint64_t uuid)
        : _uuid{uuid}
    {
    }
    operator uint64_t() const { return _uuid; }

private:
    uint64_t _uuid;
};

} // namespace Cool
