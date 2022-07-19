#include "Path.h"

namespace Cool {

std::string Path::_root;
std::string Path::_cool_res;
#if DEBUG
bool Path::_root_is_initialized     = false;
bool Path::_cool_res_is_initialized = false;
#endif

const std::string& Path::root()
{
    assert(_root_is_initialized);
    return _root;
}

void Path::initialize_root(std::string_view path)
{
#if DEBUG
    assert(!_root_is_initialized);
    _root_is_initialized = true;
#endif
    _root = path;
}

const std::string& Path::cool_res()
{
    assert(_cool_res_is_initialized);
    return _cool_res;
}

void Path::initialize_cool_res(std::string_view path)
{
#if DEBUG
    assert(!_cool_res_is_initialized);
    _cool_res_is_initialized = true;
#endif
    _cool_res = path;
}

} // namespace Cool
