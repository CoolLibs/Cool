#include "UniqueVAO.h"

namespace Cool {

UniqueVAO::UniqueVAO()
{
    GLDebug(glGenVertexArrays(1, &_id));
}

UniqueVAO::~UniqueVAO()
{
    GLDebug(glDeleteVertexArrays(1, &_id));
}

UniqueVAO::UniqueVAO(UniqueVAO&& rhs) noexcept
    : _id{rhs._id}
{
    rhs._id = decltype(rhs._id){};
}

UniqueVAO& UniqueVAO::operator=(UniqueVAO&& rhs) noexcept
{
    if (&rhs != this) {
        _id     = rhs._id;
        rhs._id = decltype(rhs._id){};
    }
    return *this;
}

} // namespace Cool
