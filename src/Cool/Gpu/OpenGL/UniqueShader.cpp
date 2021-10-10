#if defined(COOL_OPENGL)
#include "UniqueShader.h"

namespace Cool {

UniqueShader::UniqueShader()
{
    GLDebug(_id = glCreateProgram());
}

UniqueShader::~UniqueShader()
{
    GLDebug(glDeleteProgram(_id));
}

UniqueShader::UniqueShader(UniqueShader&& rhs) noexcept
    : _id{rhs._id}
{
    rhs._id = decltype(rhs._id){};
}

UniqueShader& UniqueShader::operator=(UniqueShader&& rhs) noexcept
{
    if (&rhs != this) {
        _id     = rhs._id;
        rhs._id = decltype(rhs._id){};
    }
    return *this;
}

} // namespace Cool

#endif