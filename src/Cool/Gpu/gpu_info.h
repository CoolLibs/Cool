#pragma once

namespace Cool {

// The string is cached so calling this is very cheap after the first time.
auto gpu_name() -> std::string const&;

// The string is cached so calling this is very cheap after the first time.
auto full_gpu_info() -> std::string const&;

} // namespace Cool