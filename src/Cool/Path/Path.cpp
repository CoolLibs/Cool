#include "Path.h"

namespace Cool {

std::filesystem::path Path::_root;
std::filesystem::path Path::_cool_res;
#if DEBUG
bool Path::_root_is_initialized     = false;
bool Path::_cool_res_is_initialized = false;
#endif

const std::filesystem::path& Path::root()
{
#if DEBUG // On some compilers asserts are compiled even when not in DEBUG
    assert(_root_is_initialized);
#endif
    return _root;
}

void Path::initialize_root(std::filesystem::path path)
{
#if DEBUG
    assert(!_root_is_initialized);
    _root_is_initialized = true;
#endif
    _root = path;
}

const std::filesystem::path& Path::cool_res()
{
#if DEBUG // On some compilers asserts are compiled even when not in DEBUG
    assert(_cool_res_is_initialized);
#endif
    return _cool_res;
}

void Path::initialize_cool_res(std::filesystem::path path)
{
#if DEBUG
    assert(!_cool_res_is_initialized);
    _cool_res_is_initialized = true;
#endif
    _cool_res = path;
}

} // namespace Cool
