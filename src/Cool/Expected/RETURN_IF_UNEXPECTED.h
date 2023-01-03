#pragma once

#define RETURN_IF_UNEXPECTED(x) \
    if (!x)                     \
        return tl::make_unexpected(x.error());
