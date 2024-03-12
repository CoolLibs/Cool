#pragma once

/// x should be a std::optional<std::string> representing an optional error message
#define RETURN_IF_ERROR(x)                                       \
    {                                                            \
        auto maybe_error = x;                                    \
        if (maybe_error)                                         \
            return tl::make_unexpected(std::move(*maybe_error)); \
    }

/// x should be a std::optional<std::string> representing an optional error message
#define RETURN_IF_ERR(x)        \
    {                           \
        auto maybe_error = x;   \
        if (maybe_error)        \
            return maybe_error; \
    }
