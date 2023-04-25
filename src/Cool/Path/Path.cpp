#include "Path.h"

namespace Cool {

std::filesystem::path Path::_root;
std::filesystem::path Path::_cool_res;
std::filesystem::path Path::_default_texture;
#if DEBUG
bool Path::_root_is_initialized            = false;
bool Path::_cool_res_is_initialized        = false;
bool Path::_default_texture_is_initialized = false;
#endif

auto Path::root() -> const std::filesystem::path&
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

auto Path::cool_res() -> const std::filesystem::path&
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

auto Path::default_texture() -> const std::filesystem::path&
{
#if DEBUG // On some compilers asserts are compiled even when not in DEBUG
    assert(_default_texture_is_initialized);
#endif
    return _default_texture;
}

void Path::initialize_default_texture(std::filesystem::path path)
{
#if DEBUG
    assert(!_default_texture_is_initialized);
    _default_texture_is_initialized = true;
#endif
    _default_texture = path;
}

} // namespace Cool
