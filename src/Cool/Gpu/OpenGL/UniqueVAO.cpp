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
    rhs._id = GLuint{};
}

UniqueVAO& UniqueVAO::operator=(UniqueVAO&& rhs) noexcept
{
    if (&rhs != this) {
        _id     = rhs._id;
        rhs._id = GLuint{};
    }
    return *this;
}

} // namespace Cool
